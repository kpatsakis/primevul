int rtrs_clt_request(int dir, struct rtrs_clt_req_ops *ops,
		     struct rtrs_clt_sess *clt, struct rtrs_permit *permit,
		     const struct kvec *vec, size_t nr, size_t data_len,
		     struct scatterlist *sg, unsigned int sg_cnt)
{
	struct rtrs_clt_io_req *req;
	struct rtrs_clt_path *clt_path;

	enum dma_data_direction dma_dir;
	int err = -ECONNABORTED, i;
	size_t usr_len, hdr_len;
	struct path_it it;

	/* Get kvec length */
	for (i = 0, usr_len = 0; i < nr; i++)
		usr_len += vec[i].iov_len;

	if (dir == READ) {
		hdr_len = sizeof(struct rtrs_msg_rdma_read) +
			  sg_cnt * sizeof(struct rtrs_sg_desc);
		dma_dir = DMA_FROM_DEVICE;
	} else {
		hdr_len = sizeof(struct rtrs_msg_rdma_write);
		dma_dir = DMA_TO_DEVICE;
	}

	rcu_read_lock();
	for (path_it_init(&it, clt);
	     (clt_path = it.next_path(&it)) && it.i < it.clt->paths_num; it.i++) {
		if (READ_ONCE(clt_path->state) != RTRS_CLT_CONNECTED)
			continue;

		if (usr_len + hdr_len > clt_path->max_hdr_size) {
			rtrs_wrn_rl(clt_path->clt,
				     "%s request failed, user message size is %zu and header length %zu, but max size is %u\n",
				     dir == READ ? "Read" : "Write",
				     usr_len, hdr_len, clt_path->max_hdr_size);
			err = -EMSGSIZE;
			break;
		}
		req = rtrs_clt_get_req(clt_path, ops->conf_fn, permit, ops->priv,
				       vec, usr_len, sg, sg_cnt, data_len,
				       dma_dir);
		if (dir == READ)
			err = rtrs_clt_read_req(req);
		else
			err = rtrs_clt_write_req(req);
		if (err) {
			req->in_use = false;
			continue;
		}
		/* Success path */
		break;
	}
	path_it_deinit(&it);
	rcu_read_unlock();

	return err;
}