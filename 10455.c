static void rtrs_clt_rdma_done(struct ib_cq *cq, struct ib_wc *wc)
{
	struct rtrs_clt_con *con = to_clt_con(wc->qp->qp_context);
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);
	u32 imm_type, imm_payload;
	bool w_inval = false;
	int err;

	if (wc->status != IB_WC_SUCCESS) {
		if (wc->status != IB_WC_WR_FLUSH_ERR) {
			rtrs_err(clt_path->clt, "RDMA failed: %s\n",
				  ib_wc_status_msg(wc->status));
			rtrs_rdma_error_recovery(con);
		}
		return;
	}
	rtrs_clt_update_wc_stats(con);

	switch (wc->opcode) {
	case IB_WC_RECV_RDMA_WITH_IMM:
		/*
		 * post_recv() RDMA write completions of IO reqs (read/write)
		 * and hb
		 */
		if (WARN_ON(wc->wr_cqe->done != rtrs_clt_rdma_done))
			return;
		rtrs_from_imm(be32_to_cpu(wc->ex.imm_data),
			       &imm_type, &imm_payload);
		if (imm_type == RTRS_IO_RSP_IMM ||
		    imm_type == RTRS_IO_RSP_W_INV_IMM) {
			u32 msg_id;

			w_inval = (imm_type == RTRS_IO_RSP_W_INV_IMM);
			rtrs_from_io_rsp_imm(imm_payload, &msg_id, &err);

			process_io_rsp(clt_path, msg_id, err, w_inval);
		} else if (imm_type == RTRS_HB_MSG_IMM) {
			WARN_ON(con->c.cid);
			rtrs_send_hb_ack(&clt_path->s);
			if (clt_path->flags & RTRS_MSG_NEW_RKEY_F)
				return  rtrs_clt_recv_done(con, wc);
		} else if (imm_type == RTRS_HB_ACK_IMM) {
			WARN_ON(con->c.cid);
			clt_path->s.hb_missed_cnt = 0;
			clt_path->s.hb_cur_latency =
				ktime_sub(ktime_get(), clt_path->s.hb_last_sent);
			if (clt_path->flags & RTRS_MSG_NEW_RKEY_F)
				return  rtrs_clt_recv_done(con, wc);
		} else {
			rtrs_wrn(con->c.path, "Unknown IMM type %u\n",
				  imm_type);
		}
		if (w_inval)
			/*
			 * Post x2 empty WRs: first is for this RDMA with IMM,
			 * second is for RECV with INV, which happened earlier.
			 */
			err = rtrs_post_recv_empty_x2(&con->c, &io_comp_cqe);
		else
			err = rtrs_post_recv_empty(&con->c, &io_comp_cqe);
		if (err) {
			rtrs_err(con->c.path, "rtrs_post_recv_empty(): %d\n",
				  err);
			rtrs_rdma_error_recovery(con);
		}
		break;
	case IB_WC_RECV:
		/*
		 * Key invalidations from server side
		 */
		WARN_ON(!(wc->wc_flags & IB_WC_WITH_INVALIDATE ||
			  wc->wc_flags & IB_WC_WITH_IMM));
		WARN_ON(wc->wr_cqe->done != rtrs_clt_rdma_done);
		if (clt_path->flags & RTRS_MSG_NEW_RKEY_F) {
			if (wc->wc_flags & IB_WC_WITH_INVALIDATE)
				return  rtrs_clt_recv_done(con, wc);

			return  rtrs_clt_rkey_rsp_done(con, wc);
		}
		break;
	case IB_WC_RDMA_WRITE:
		/*
		 * post_send() RDMA write completions of IO reqs (read/write)
		 * and hb.
		 */
		break;

	default:
		rtrs_wrn(clt_path->clt, "Unexpected WC type: %d\n", wc->opcode);
		return;
	}
}