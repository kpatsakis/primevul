static int rtrs_clt_read_req(struct rtrs_clt_io_req *req)
{
	struct rtrs_clt_con *con = req->con;
	struct rtrs_path *s = con->c.path;
	struct rtrs_clt_path *clt_path = to_clt_path(s);
	struct rtrs_msg_rdma_read *msg;
	struct rtrs_ib_dev *dev = clt_path->s.dev;

	struct ib_reg_wr rwr;
	struct ib_send_wr *wr = NULL;

	int ret, count = 0;
	u32 imm, buf_id;

	const size_t tsize = sizeof(*msg) + req->data_len + req->usr_len;

	if (tsize > clt_path->chunk_size) {
		rtrs_wrn(s,
			  "Read request failed, message size is %zu, bigger than CHUNK_SIZE %d\n",
			  tsize, clt_path->chunk_size);
		return -EMSGSIZE;
	}

	if (req->sg_cnt) {
		count = ib_dma_map_sg(dev->ib_dev, req->sglist, req->sg_cnt,
				      req->dir);
		if (!count) {
			rtrs_wrn(s,
				  "Read request failed, dma map failed\n");
			return -EINVAL;
		}
	}
	/* put our message into req->buf after user message*/
	msg = req->iu->buf + req->usr_len;
	msg->type = cpu_to_le16(RTRS_MSG_READ);
	msg->usr_len = cpu_to_le16(req->usr_len);

	if (count) {
		ret = rtrs_map_sg_fr(req, count);
		if (ret < 0) {
			rtrs_err_rl(s,
				     "Read request failed, failed to map  fast reg. data, err: %d\n",
				     ret);
			ib_dma_unmap_sg(dev->ib_dev, req->sglist, req->sg_cnt,
					req->dir);
			return ret;
		}
		rwr = (struct ib_reg_wr) {
			.wr.opcode = IB_WR_REG_MR,
			.wr.wr_cqe = &fast_reg_cqe,
			.mr = req->mr,
			.key = req->mr->rkey,
			.access = (IB_ACCESS_LOCAL_WRITE |
				   IB_ACCESS_REMOTE_WRITE),
		};
		wr = &rwr.wr;

		msg->sg_cnt = cpu_to_le16(1);
		msg->flags = cpu_to_le16(RTRS_MSG_NEED_INVAL_F);

		msg->desc[0].addr = cpu_to_le64(req->mr->iova);
		msg->desc[0].key = cpu_to_le32(req->mr->rkey);
		msg->desc[0].len = cpu_to_le32(req->mr->length);

		/* Further invalidation is required */
		req->need_inv = !!RTRS_MSG_NEED_INVAL_F;

	} else {
		msg->sg_cnt = 0;
		msg->flags = 0;
	}
	/*
	 * rtrs message will be after the space reserved for disk data and
	 * user message
	 */
	imm = req->permit->mem_off + req->data_len + req->usr_len;
	imm = rtrs_to_io_req_imm(imm);
	buf_id = req->permit->mem_id;

	req->sg_size  = sizeof(*msg);
	req->sg_size += le16_to_cpu(msg->sg_cnt) * sizeof(struct rtrs_sg_desc);
	req->sg_size += req->usr_len;

	/*
	 * Update stats now, after request is successfully sent it is not
	 * safe anymore to touch it.
	 */
	rtrs_clt_update_all_stats(req, READ);

	ret = rtrs_post_send_rdma(req->con, req, &clt_path->rbufs[buf_id],
				   req->data_len, imm, wr);
	if (ret) {
		rtrs_err_rl(s,
			    "Read request failed: error=%d path=%s [%s:%u]\n",
			    ret, kobject_name(&clt_path->kobj), clt_path->hca_name,
			    clt_path->hca_port);
		if (req->mp_policy == MP_POLICY_MIN_INFLIGHT)
			atomic_dec(&clt_path->stats->inflight);
		req->need_inv = false;
		if (req->sg_cnt)
			ib_dma_unmap_sg(dev->ib_dev, req->sglist,
					req->sg_cnt, req->dir);
	}

	return ret;
}