rtrs_clt_get_req(struct rtrs_clt_path *clt_path,
		 void (*conf)(void *priv, int errno),
		 struct rtrs_permit *permit, void *priv,
		 const struct kvec *vec, size_t usr_len,
		 struct scatterlist *sg, size_t sg_cnt,
		 size_t data_len, int dir)
{
	struct rtrs_clt_io_req *req;

	req = &clt_path->reqs[permit->mem_id];
	rtrs_clt_init_req(req, clt_path, conf, permit, priv, vec, usr_len,
			   sg, sg_cnt, data_len, dir);
	return req;
}