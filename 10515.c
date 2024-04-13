static void rtrs_clt_recv_done(struct rtrs_clt_con *con, struct ib_wc *wc)
{
	struct rtrs_iu *iu;
	int err;
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);

	WARN_ON((clt_path->flags & RTRS_MSG_NEW_RKEY_F) == 0);
	iu = container_of(wc->wr_cqe, struct rtrs_iu,
			  cqe);
	err = rtrs_iu_post_recv(&con->c, iu);
	if (err) {
		rtrs_err(con->c.path, "post iu failed %d\n", err);
		rtrs_rdma_error_recovery(con);
	}
}