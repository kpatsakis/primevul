static int rtrs_rdma_conn_rejected(struct rtrs_clt_con *con,
				    struct rdma_cm_event *ev)
{
	struct rtrs_path *s = con->c.path;
	const struct rtrs_msg_conn_rsp *msg;
	const char *rej_msg;
	int status, errno;
	u8 data_len;

	status = ev->status;
	rej_msg = rdma_reject_msg(con->c.cm_id, status);
	msg = rdma_consumer_reject_data(con->c.cm_id, ev, &data_len);

	if (msg && data_len >= sizeof(*msg)) {
		errno = (int16_t)le16_to_cpu(msg->errno);
		if (errno == -EBUSY)
			rtrs_err(s,
				  "Previous session is still exists on the server, please reconnect later\n");
		else
			rtrs_err(s,
				  "Connect rejected: status %d (%s), rtrs errno %d\n",
				  status, rej_msg, errno);
	} else {
		rtrs_err(s,
			  "Connect rejected but with malformed message: status %d (%s)\n",
			  status, rej_msg);
	}

	return -ECONNRESET;
}