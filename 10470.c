static void rtrs_rdma_error_recovery(struct rtrs_clt_con *con)
{
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);

	if (rtrs_clt_change_state_from_to(clt_path,
					   RTRS_CLT_CONNECTED,
					   RTRS_CLT_RECONNECTING)) {
		struct rtrs_clt_sess *clt = clt_path->clt;
		unsigned int delay_ms;

		/*
		 * Normal scenario, reconnect if we were successfully connected
		 */
		delay_ms = clt->reconnect_delay_sec * 1000;
		queue_delayed_work(rtrs_wq, &clt_path->reconnect_dwork,
				   msecs_to_jiffies(delay_ms +
						    prandom_u32() % RTRS_RECONNECT_SEED));
	} else {
		/*
		 * Error can happen just on establishing new connection,
		 * so notify waiter with error state, waiter is responsible
		 * for cleaning the rest and reconnect if needed.
		 */
		rtrs_clt_change_state_from_to(clt_path,
					       RTRS_CLT_CONNECTING,
					       RTRS_CLT_CONNECTING_ERR);
	}
}