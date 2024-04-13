static void rtrs_clt_init_hb(struct rtrs_clt_path *clt_path)
{
	rtrs_init_hb(&clt_path->s, &io_comp_cqe,
		      RTRS_HB_INTERVAL_MS,
		      RTRS_HB_MISSED_MAX,
		      rtrs_clt_hb_err_handler,
		      rtrs_wq);
}