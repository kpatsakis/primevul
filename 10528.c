static void rtrs_clt_info_req_done(struct ib_cq *cq, struct ib_wc *wc)
{
	struct rtrs_clt_con *con = to_clt_con(wc->qp->qp_context);
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);
	struct rtrs_iu *iu;

	iu = container_of(wc->wr_cqe, struct rtrs_iu, cqe);
	rtrs_iu_free(iu, clt_path->s.dev->ib_dev, 1);

	if (wc->status != IB_WC_SUCCESS) {
		rtrs_err(clt_path->clt, "Path info request send failed: %s\n",
			  ib_wc_status_msg(wc->status));
		rtrs_clt_change_state_get_old(clt_path, RTRS_CLT_CONNECTING_ERR, NULL);
		return;
	}

	rtrs_clt_update_wc_stats(con);
}