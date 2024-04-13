static int rtrs_map_sg_fr(struct rtrs_clt_io_req *req, size_t count)
{
	int nr;

	/* Align the MR to a 4K page size to match the block virt boundary */
	nr = ib_map_mr_sg(req->mr, req->sglist, count, NULL, SZ_4K);
	if (nr < 0)
		return nr;
	if (nr < req->sg_cnt)
		return -EINVAL;
	ib_update_fast_reg_key(req->mr, ib_inc_rkey(req->mr->rkey));

	return nr;
}