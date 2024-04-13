int rtrs_clt_reconnect_from_sysfs(struct rtrs_clt_path *clt_path)
{
	enum rtrs_clt_state old_state;
	int err = -EBUSY;
	bool changed;

	changed = rtrs_clt_change_state_get_old(clt_path,
						 RTRS_CLT_RECONNECTING,
						 &old_state);
	if (changed) {
		clt_path->reconnect_attempts = 0;
		queue_delayed_work(rtrs_wq, &clt_path->reconnect_dwork, 0);
	}
	if (changed || old_state == RTRS_CLT_RECONNECTING) {
		/*
		 * flush_delayed_work() queues pending work for immediate
		 * execution, so do the flush if we have queued something
		 * right now or work is pending.
		 */
		flush_delayed_work(&clt_path->reconnect_dwork);
		err = (READ_ONCE(clt_path->state) ==
		       RTRS_CLT_CONNECTED ? 0 : -ENOTCONN);
	}

	return err;
}