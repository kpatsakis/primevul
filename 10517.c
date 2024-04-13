static void free_path_reqs(struct rtrs_clt_path *clt_path)
{
	struct rtrs_clt_io_req *req;
	int i;

	if (!clt_path->reqs)
		return;
	for (i = 0; i < clt_path->queue_depth; ++i) {
		req = &clt_path->reqs[i];
		if (req->mr)
			ib_dereg_mr(req->mr);
		kfree(req->sge);
		rtrs_iu_free(req->iu, clt_path->s.dev->ib_dev, 1);
	}
	kfree(clt_path->reqs);
	clt_path->reqs = NULL;
}