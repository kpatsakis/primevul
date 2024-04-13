static int alloc_path_reqs(struct rtrs_clt_path *clt_path)
{
	struct rtrs_clt_io_req *req;
	int i, err = -ENOMEM;

	clt_path->reqs = kcalloc(clt_path->queue_depth,
				 sizeof(*clt_path->reqs),
				 GFP_KERNEL);
	if (!clt_path->reqs)
		return -ENOMEM;

	for (i = 0; i < clt_path->queue_depth; ++i) {
		req = &clt_path->reqs[i];
		req->iu = rtrs_iu_alloc(1, clt_path->max_hdr_size, GFP_KERNEL,
					 clt_path->s.dev->ib_dev,
					 DMA_TO_DEVICE,
					 rtrs_clt_rdma_done);
		if (!req->iu)
			goto out;

		req->sge = kcalloc(2, sizeof(*req->sge), GFP_KERNEL);
		if (!req->sge)
			goto out;

		req->mr = ib_alloc_mr(clt_path->s.dev->ib_pd,
				      IB_MR_TYPE_MEM_REG,
				      clt_path->max_pages_per_mr);
		if (IS_ERR(req->mr)) {
			err = PTR_ERR(req->mr);
			req->mr = NULL;
			pr_err("Failed to alloc clt_path->max_pages_per_mr %d\n",
			       clt_path->max_pages_per_mr);
			goto out;
		}

		init_completion(&req->inv_comp);
	}

	return 0;

out:
	free_path_reqs(clt_path);

	return err;
}