cifs_call_async(struct TCP_Server_Info *server, struct smb_rqst *rqst,
		mid_receive_t *receive, mid_callback_t *callback,
		void *cbdata, const int flags)
{
	int rc, timeout, optype;
	struct mid_q_entry *mid;

	timeout = flags & CIFS_TIMEOUT_MASK;
	optype = flags & CIFS_OP_MASK;

	rc = wait_for_free_request(server, timeout, optype);
	if (rc)
		return rc;

	mutex_lock(&server->srv_mutex);
	mid = server->ops->setup_async_request(server, rqst);
	if (IS_ERR(mid)) {
		mutex_unlock(&server->srv_mutex);
		add_credits(server, 1, optype);
		wake_up(&server->request_q);
		return PTR_ERR(mid);
	}

	mid->receive = receive;
	mid->callback = callback;
	mid->callback_data = cbdata;
	mid->mid_state = MID_REQUEST_SUBMITTED;

	/* put it on the pending_mid_q */
	spin_lock(&GlobalMid_Lock);
	list_add_tail(&mid->qhead, &server->pending_mid_q);
	spin_unlock(&GlobalMid_Lock);


	cifs_in_send_inc(server);
	rc = smb_send_rqst(server, rqst);
	cifs_in_send_dec(server);
	cifs_save_when_sent(mid);
	mutex_unlock(&server->srv_mutex);

	if (rc == 0)
		return 0;

	cifs_delete_mid(mid);
	add_credits(server, 1, optype);
	wake_up(&server->request_q);
	return rc;
}
