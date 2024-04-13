static void rtrs_clt_reconnect_work(struct work_struct *work)
{
	struct rtrs_clt_path *clt_path;
	struct rtrs_clt_sess *clt;
	unsigned int delay_ms;
	int err;

	clt_path = container_of(to_delayed_work(work), struct rtrs_clt_path,
				reconnect_dwork);
	clt = clt_path->clt;

	if (READ_ONCE(clt_path->state) != RTRS_CLT_RECONNECTING)
		return;

	if (clt_path->reconnect_attempts >= clt->max_reconnect_attempts) {
		/* Close a path completely if max attempts is reached */
		rtrs_clt_close_conns(clt_path, false);
		return;
	}
	clt_path->reconnect_attempts++;

	/* Stop everything */
	rtrs_clt_stop_and_destroy_conns(clt_path);
	msleep(RTRS_RECONNECT_BACKOFF);
	if (rtrs_clt_change_state_get_old(clt_path, RTRS_CLT_CONNECTING, NULL)) {
		err = init_path(clt_path);
		if (err)
			goto reconnect_again;
	}

	return;

reconnect_again:
	if (rtrs_clt_change_state_get_old(clt_path, RTRS_CLT_RECONNECTING, NULL)) {
		clt_path->stats->reconnects.fail_cnt++;
		delay_ms = clt->reconnect_delay_sec * 1000;
		queue_delayed_work(rtrs_wq, &clt_path->reconnect_dwork,
				   msecs_to_jiffies(delay_ms +
						    prandom_u32() %
						    RTRS_RECONNECT_SEED));
	}
}