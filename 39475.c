SendReceive2(const unsigned int xid, struct cifs_ses *ses,
	     struct kvec *iov, int n_vec, int *resp_buf_type /* ret */,
	     const int flags)
{
	int rc = 0;
	int timeout, optype;
	struct mid_q_entry *midQ;
	char *buf = iov[0].iov_base;
	unsigned int credits = 1;
	struct smb_rqst rqst = { .rq_iov = iov,
				 .rq_nvec = n_vec };

	timeout = flags & CIFS_TIMEOUT_MASK;
	optype = flags & CIFS_OP_MASK;

	*resp_buf_type = CIFS_NO_BUFFER;  /* no response buf yet */

	if ((ses == NULL) || (ses->server == NULL)) {
		cifs_small_buf_release(buf);
		cERROR(1, "Null session");
		return -EIO;
	}

	if (ses->server->tcpStatus == CifsExiting) {
		cifs_small_buf_release(buf);
		return -ENOENT;
	}

	/*
	 * Ensure that we do not send more than 50 overlapping requests
	 * to the same server. We may make this configurable later or
	 * use ses->maxReq.
	 */

	rc = wait_for_free_request(ses->server, timeout, optype);
	if (rc) {
		cifs_small_buf_release(buf);
		return rc;
	}

	/*
	 * Make sure that we sign in the same order that we send on this socket
	 * and avoid races inside tcp sendmsg code that could cause corruption
	 * of smb data.
	 */

	mutex_lock(&ses->server->srv_mutex);

	midQ = ses->server->ops->setup_request(ses, &rqst);
	if (IS_ERR(midQ)) {
		mutex_unlock(&ses->server->srv_mutex);
		cifs_small_buf_release(buf);
		/* Update # of requests on wire to server */
		add_credits(ses->server, 1, optype);
		return PTR_ERR(midQ);
	}

	midQ->mid_state = MID_REQUEST_SUBMITTED;
	cifs_in_send_inc(ses->server);
	rc = smb_sendv(ses->server, iov, n_vec);
	cifs_in_send_dec(ses->server);
	cifs_save_when_sent(midQ);

	mutex_unlock(&ses->server->srv_mutex);

	if (rc < 0) {
		cifs_small_buf_release(buf);
		goto out;
	}

	if (timeout == CIFS_ASYNC_OP) {
		cifs_small_buf_release(buf);
		goto out;
	}

	rc = wait_for_response(ses->server, midQ);
	if (rc != 0) {
		send_cancel(ses->server, buf, midQ);
		spin_lock(&GlobalMid_Lock);
		if (midQ->mid_state == MID_REQUEST_SUBMITTED) {
			midQ->callback = DeleteMidQEntry;
			spin_unlock(&GlobalMid_Lock);
			cifs_small_buf_release(buf);
			add_credits(ses->server, 1, optype);
			return rc;
		}
		spin_unlock(&GlobalMid_Lock);
	}

	cifs_small_buf_release(buf);

	rc = cifs_sync_mid_result(midQ, ses->server);
	if (rc != 0) {
		add_credits(ses->server, 1, optype);
		return rc;
	}

	if (!midQ->resp_buf || midQ->mid_state != MID_RESPONSE_RECEIVED) {
		rc = -EIO;
		cFYI(1, "Bad MID state?");
		goto out;
	}

	buf = (char *)midQ->resp_buf;
	iov[0].iov_base = buf;
	iov[0].iov_len = get_rfc1002_length(buf) + 4;
	if (midQ->large_buf)
		*resp_buf_type = CIFS_LARGE_BUFFER;
	else
		*resp_buf_type = CIFS_SMALL_BUFFER;

	credits = ses->server->ops->get_credits(midQ);

	rc = ses->server->ops->check_receive(midQ, ses->server,
					     flags & CIFS_LOG_ERROR);

	/* mark it so buf will not be freed by cifs_delete_mid */
	if ((flags & CIFS_NO_RESP) == 0)
		midQ->resp_buf = NULL;
out:
	cifs_delete_mid(midQ);
	add_credits(ses->server, credits, optype);

	return rc;
}
