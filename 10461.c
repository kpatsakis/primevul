void rtrs_clt_close_conns(struct rtrs_clt_path *clt_path, bool wait)
{
	if (rtrs_clt_change_state_get_old(clt_path, RTRS_CLT_CLOSING, NULL))
		queue_work(rtrs_wq, &clt_path->close_work);
	if (wait)
		flush_work(&clt_path->close_work);
}