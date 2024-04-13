static void rtrs_clt_init_req(struct rtrs_clt_io_req *req,
			      struct rtrs_clt_path *clt_path,
			      void (*conf)(void *priv, int errno),
			      struct rtrs_permit *permit, void *priv,
			      const struct kvec *vec, size_t usr_len,
			      struct scatterlist *sg, size_t sg_cnt,
			      size_t data_len, int dir)
{
	struct iov_iter iter;
	size_t len;

	req->permit = permit;
	req->in_use = true;
	req->usr_len = usr_len;
	req->data_len = data_len;
	req->sglist = sg;
	req->sg_cnt = sg_cnt;
	req->priv = priv;
	req->dir = dir;
	req->con = rtrs_permit_to_clt_con(clt_path, permit);
	req->conf = conf;
	req->need_inv = false;
	req->need_inv_comp = false;
	req->inv_errno = 0;
	refcount_set(&req->ref, 1);
	req->mp_policy = clt_path->clt->mp_policy;

	iov_iter_kvec(&iter, READ, vec, 1, usr_len);
	len = _copy_from_iter(req->iu->buf, usr_len, &iter);
	WARN_ON(len != usr_len);

	reinit_completion(&req->inv_comp);
}