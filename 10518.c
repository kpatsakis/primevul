rtrs_clt_get_copy_req(struct rtrs_clt_path *alive_path,
		       struct rtrs_clt_io_req *fail_req)
{
	struct rtrs_clt_io_req *req;
	struct kvec vec = {
		.iov_base = fail_req->iu->buf,
		.iov_len  = fail_req->usr_len
	};

	req = &alive_path->reqs[fail_req->permit->mem_id];
	rtrs_clt_init_req(req, alive_path, fail_req->conf, fail_req->permit,
			   fail_req->priv, &vec, fail_req->usr_len,
			   fail_req->sglist, fail_req->sg_cnt,
			   fail_req->data_len, fail_req->dir);
	return req;
}