static void rtrs_clt_close_work(struct work_struct *work)
{
	struct rtrs_clt_path *clt_path;

	clt_path = container_of(work, struct rtrs_clt_path, close_work);

	cancel_delayed_work_sync(&clt_path->reconnect_dwork);
	rtrs_clt_stop_and_destroy_conns(clt_path);
	rtrs_clt_change_state_get_old(clt_path, RTRS_CLT_CLOSED, NULL);
}