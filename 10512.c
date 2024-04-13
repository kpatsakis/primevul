static void rtrs_clt_inv_rkey_done(struct ib_cq *cq, struct ib_wc *wc)
{
	struct rtrs_clt_io_req *req =
		container_of(wc->wr_cqe, typeof(*req), inv_cqe);
	struct rtrs_clt_con *con = to_clt_con(wc->qp->qp_context);

	if (wc->status != IB_WC_SUCCESS) {
		rtrs_err(con->c.path, "Failed IB_WR_LOCAL_INV: %s\n",
			  ib_wc_status_msg(wc->status));
		rtrs_rdma_error_recovery(con);
	}
	req->need_inv = false;
	if (req->need_inv_comp)
		complete(&req->inv_comp);
	else
		/* Complete request from INV callback */
		complete_rdma_req(req, req->inv_errno, true, false);
}