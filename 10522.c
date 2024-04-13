static int post_recv_path(struct rtrs_clt_path *clt_path)
{
	size_t q_size = 0;
	int err, cid;

	for (cid = 0; cid < clt_path->s.con_num; cid++) {
		if (cid == 0)
			q_size = SERVICE_CON_QUEUE_DEPTH;
		else
			q_size = clt_path->queue_depth;

		/*
		 * x2 for RDMA read responses + FR key invalidations,
		 * RDMA writes do not require any FR registrations.
		 */
		q_size *= 2;

		err = post_recv_io(to_clt_con(clt_path->s.con[cid]), q_size);
		if (err) {
			rtrs_err(clt_path->clt, "post_recv_io(), err: %d\n",
				 err);
			return err;
		}
	}

	return 0;
}