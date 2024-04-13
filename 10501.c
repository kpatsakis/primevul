static void rtrs_clt_rkey_rsp_done(struct rtrs_clt_con *con, struct ib_wc *wc)
{
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);
	struct rtrs_msg_rkey_rsp *msg;
	u32 imm_type, imm_payload;
	bool w_inval = false;
	struct rtrs_iu *iu;
	u32 buf_id;
	int err;

	WARN_ON((clt_path->flags & RTRS_MSG_NEW_RKEY_F) == 0);

	iu = container_of(wc->wr_cqe, struct rtrs_iu, cqe);

	if (wc->byte_len < sizeof(*msg)) {
		rtrs_err(con->c.path, "rkey response is malformed: size %d\n",
			  wc->byte_len);
		goto out;
	}
	ib_dma_sync_single_for_cpu(clt_path->s.dev->ib_dev, iu->dma_addr,
				   iu->size, DMA_FROM_DEVICE);
	msg = iu->buf;
	if (le16_to_cpu(msg->type) != RTRS_MSG_RKEY_RSP) {
		rtrs_err(clt_path->clt,
			  "rkey response is malformed: type %d\n",
			  le16_to_cpu(msg->type));
		goto out;
	}
	buf_id = le16_to_cpu(msg->buf_id);
	if (WARN_ON(buf_id >= clt_path->queue_depth))
		goto out;

	rtrs_from_imm(be32_to_cpu(wc->ex.imm_data), &imm_type, &imm_payload);
	if (imm_type == RTRS_IO_RSP_IMM ||
	    imm_type == RTRS_IO_RSP_W_INV_IMM) {
		u32 msg_id;

		w_inval = (imm_type == RTRS_IO_RSP_W_INV_IMM);
		rtrs_from_io_rsp_imm(imm_payload, &msg_id, &err);

		if (WARN_ON(buf_id != msg_id))
			goto out;
		clt_path->rbufs[buf_id].rkey = le32_to_cpu(msg->rkey);
		process_io_rsp(clt_path, msg_id, err, w_inval);
	}
	ib_dma_sync_single_for_device(clt_path->s.dev->ib_dev, iu->dma_addr,
				      iu->size, DMA_FROM_DEVICE);
	return rtrs_clt_recv_done(con, wc);
out:
	rtrs_rdma_error_recovery(con);
}