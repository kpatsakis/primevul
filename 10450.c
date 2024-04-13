static int create_cm(struct rtrs_clt_con *con)
{
	struct rtrs_path *s = con->c.path;
	struct rtrs_clt_path *clt_path = to_clt_path(s);
	struct rdma_cm_id *cm_id;
	int err;

	cm_id = rdma_create_id(&init_net, rtrs_clt_rdma_cm_handler, con,
			       clt_path->s.dst_addr.ss_family == AF_IB ?
			       RDMA_PS_IB : RDMA_PS_TCP, IB_QPT_RC);
	if (IS_ERR(cm_id)) {
		err = PTR_ERR(cm_id);
		rtrs_err(s, "Failed to create CM ID, err: %d\n", err);

		return err;
	}
	con->c.cm_id = cm_id;
	con->cm_err = 0;
	/* allow the port to be reused */
	err = rdma_set_reuseaddr(cm_id, 1);
	if (err != 0) {
		rtrs_err(s, "Set address reuse failed, err: %d\n", err);
		goto destroy_cm;
	}
	err = rdma_resolve_addr(cm_id, (struct sockaddr *)&clt_path->s.src_addr,
				(struct sockaddr *)&clt_path->s.dst_addr,
				RTRS_CONNECT_TIMEOUT_MS);
	if (err) {
		rtrs_err(s, "Failed to resolve address, err: %d\n", err);
		goto destroy_cm;
	}
	/*
	 * Combine connection status and session events. This is needed
	 * for waiting two possible cases: cm_err has something meaningful
	 * or session state was really changed to error by device removal.
	 */
	err = wait_event_interruptible_timeout(
			clt_path->state_wq,
			con->cm_err || clt_path->state != RTRS_CLT_CONNECTING,
			msecs_to_jiffies(RTRS_CONNECT_TIMEOUT_MS));
	if (err == 0 || err == -ERESTARTSYS) {
		if (err == 0)
			err = -ETIMEDOUT;
		/* Timedout or interrupted */
		goto errr;
	}
	if (con->cm_err < 0) {
		err = con->cm_err;
		goto errr;
	}
	if (READ_ONCE(clt_path->state) != RTRS_CLT_CONNECTING) {
		/* Device removal */
		err = -ECONNABORTED;
		goto errr;
	}

	return 0;

errr:
	stop_cm(con);
	mutex_lock(&con->con_mutex);
	destroy_con_cq_qp(con);
	mutex_unlock(&con->con_mutex);
destroy_cm:
	destroy_cm(con);

	return err;
}