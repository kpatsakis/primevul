static int rtrs_clt_write_req(struct rtrs_clt_io_req *req)
{
	struct rtrs_clt_con *con = req->con;
	struct rtrs_path *s = con->c.path;
	struct rtrs_clt_path *clt_path = to_clt_path(s);
	struct rtrs_msg_rdma_write *msg;

	struct rtrs_rbuf *rbuf;
	int ret, count = 0;
	u32 imm, buf_id;
	struct ib_reg_wr rwr;
	struct ib_send_wr inv_wr;
	struct ib_send_wr *wr = NULL;
	bool fr_en = false;

	const size_t tsize = sizeof(*msg) + req->data_len + req->usr_len;

	if (tsize > clt_path->chunk_size) {
		rtrs_wrn(s, "Write request failed, size too big %zu > %d\n",
			  tsize, clt_path->chunk_size);
		return -EMSGSIZE;
	}
	if (req->sg_cnt) {
		count = ib_dma_map_sg(clt_path->s.dev->ib_dev, req->sglist,
				      req->sg_cnt, req->dir);
		if (!count) {
			rtrs_wrn(s, "Write request failed, map failed\n");
			return -EINVAL;
		}
	}
	/* put rtrs msg after sg and user message */
	msg = req->iu->buf + req->usr_len;
	msg->type = cpu_to_le16(RTRS_MSG_WRITE);
	msg->usr_len = cpu_to_le16(req->usr_len);

	/* rtrs message on server side will be after user data and message */
	imm = req->permit->mem_off + req->data_len + req->usr_len;
	imm = rtrs_to_io_req_imm(imm);
	buf_id = req->permit->mem_id;
	req->sg_size = tsize;
	rbuf = &clt_path->rbufs[buf_id];

	if (count) {
		ret = rtrs_map_sg_fr(req, count);
		if (ret < 0) {
			rtrs_err_rl(s,
				    "Write request failed, failed to map fast reg. data, err: %d\n",
				    ret);
			ib_dma_unmap_sg(clt_path->s.dev->ib_dev, req->sglist,
					req->sg_cnt, req->dir);
			return ret;
		}
		inv_wr = (struct ib_send_wr) {
			.opcode		    = IB_WR_LOCAL_INV,
			.wr_cqe		    = &req->inv_cqe,
			.send_flags	    = IB_SEND_SIGNALED,
			.ex.invalidate_rkey = req->mr->rkey,
		};
		req->inv_cqe.done = rtrs_clt_inv_rkey_done;
		rwr = (struct ib_reg_wr) {
			.wr.opcode = IB_WR_REG_MR,
			.wr.wr_cqe = &fast_reg_cqe,
			.mr = req->mr,
			.key = req->mr->rkey,
			.access = (IB_ACCESS_LOCAL_WRITE),
		};
		wr = &rwr.wr;
		fr_en = true;
		refcount_inc(&req->ref);
	}
	/*
	 * Update stats now, after request is successfully sent it is not
	 * safe anymore to touch it.
	 */
	rtrs_clt_update_all_stats(req, WRITE);

	ret = rtrs_post_rdma_write_sg(req->con, req, rbuf, fr_en,
				      req->usr_len + sizeof(*msg),
				      imm, wr, &inv_wr);
	if (ret) {
		rtrs_err_rl(s,
			    "Write request failed: error=%d path=%s [%s:%u]\n",
			    ret, kobject_name(&clt_path->kobj), clt_path->hca_name,
			    clt_path->hca_port);
		if (req->mp_policy == MP_POLICY_MIN_INFLIGHT)
			atomic_dec(&clt_path->stats->inflight);
		if (req->sg_cnt)
			ib_dma_unmap_sg(clt_path->s.dev->ib_dev, req->sglist,
					req->sg_cnt, req->dir);
	}

	return ret;
}