static void rtrs_clt_info_rsp_done(struct ib_cq *cq, struct ib_wc *wc)
{
	struct rtrs_clt_con *con = to_clt_con(wc->qp->qp_context);
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);
	struct rtrs_msg_info_rsp *msg;
	enum rtrs_clt_state state;
	struct rtrs_iu *iu;
	size_t rx_sz;
	int err;

	state = RTRS_CLT_CONNECTING_ERR;

	WARN_ON(con->c.cid);
	iu = container_of(wc->wr_cqe, struct rtrs_iu, cqe);
	if (wc->status != IB_WC_SUCCESS) {
		rtrs_err(clt_path->clt, "Path info response recv failed: %s\n",
			  ib_wc_status_msg(wc->status));
		goto out;
	}
	WARN_ON(wc->opcode != IB_WC_RECV);

	if (wc->byte_len < sizeof(*msg)) {
		rtrs_err(clt_path->clt, "Path info response is malformed: size %d\n",
			  wc->byte_len);
		goto out;
	}
	ib_dma_sync_single_for_cpu(clt_path->s.dev->ib_dev, iu->dma_addr,
				   iu->size, DMA_FROM_DEVICE);
	msg = iu->buf;
	if (le16_to_cpu(msg->type) != RTRS_MSG_INFO_RSP) {
		rtrs_err(clt_path->clt, "Path info response is malformed: type %d\n",
			  le16_to_cpu(msg->type));
		goto out;
	}
	rx_sz  = sizeof(*msg);
	rx_sz += sizeof(msg->desc[0]) * le16_to_cpu(msg->sg_cnt);
	if (wc->byte_len < rx_sz) {
		rtrs_err(clt_path->clt, "Path info response is malformed: size %d\n",
			  wc->byte_len);
		goto out;
	}
	err = process_info_rsp(clt_path, msg);
	if (err)
		goto out;

	err = post_recv_path(clt_path);
	if (err)
		goto out;

	state = RTRS_CLT_CONNECTED;

out:
	rtrs_clt_update_wc_stats(con);
	rtrs_iu_free(iu, clt_path->s.dev->ib_dev, 1);
	rtrs_clt_change_state_get_old(clt_path, state, NULL);
}