static void rtrs_clt_fast_reg_done(struct ib_cq *cq, struct ib_wc *wc)
{
	struct rtrs_clt_con *con = to_clt_con(wc->qp->qp_context);

	if (wc->status != IB_WC_SUCCESS) {
		rtrs_err(con->c.path, "Failed IB_WR_REG_MR: %s\n",
			  ib_wc_status_msg(wc->status));
		rtrs_rdma_error_recovery(con);
	}
}