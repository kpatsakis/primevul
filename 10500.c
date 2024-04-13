static void fail_all_outstanding_reqs(struct rtrs_clt_path *clt_path)
{
	struct rtrs_clt_sess *clt = clt_path->clt;
	struct rtrs_clt_io_req *req;
	int i, err;

	if (!clt_path->reqs)
		return;
	for (i = 0; i < clt_path->queue_depth; ++i) {
		req = &clt_path->reqs[i];
		if (!req->in_use)
			continue;

		/*
		 * Safely (without notification) complete failed request.
		 * After completion this request is still useble and can
		 * be failovered to another path.
		 */
		complete_rdma_req(req, -ECONNABORTED, false, true);

		err = rtrs_clt_failover_req(clt, req);
		if (err)
			/* Failover failed, notify anyway */
			req->conf(req->priv, err);
	}
}