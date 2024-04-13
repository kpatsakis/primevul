static void process_io_rsp(struct rtrs_clt_path *clt_path, u32 msg_id,
			   s16 errno, bool w_inval)
{
	struct rtrs_clt_io_req *req;

	if (WARN_ON(msg_id >= clt_path->queue_depth))
		return;

	req = &clt_path->reqs[msg_id];
	/* Drop need_inv if server responded with send with invalidation */
	req->need_inv &= !w_inval;
	complete_rdma_req(req, errno, true, false);
}