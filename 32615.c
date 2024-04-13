static void stream_int_chk_snd_conn(struct stream_interface *si)
{
	struct channel *ob = si->ob;
	struct connection *conn = __objt_conn(si->end);

	if (unlikely(si->state > SI_ST_EST || (ob->flags & CF_SHUTW)))
		return;

	if (unlikely(channel_is_empty(ob)))  /* called with nothing to send ! */
		return;

	if (!ob->pipe &&                          /* spliced data wants to be forwarded ASAP */
	    !(si->flags & SI_FL_WAIT_DATA))       /* not waiting for data */
		return;

	if (conn->flags & (CO_FL_DATA_WR_ENA|CO_FL_CURR_WR_ENA)) {
		/* already subscribed to write notifications, will be called
		 * anyway, so let's avoid calling it especially if the reader
		 * is not ready.
		 */
		return;
	}

	/* Before calling the data-level operations, we have to prepare
	 * the polling flags to ensure we properly detect changes.
	 */
	conn_refresh_polling_flags(conn);
	__conn_data_want_send(conn);

	if (!(conn->flags & (CO_FL_HANDSHAKE|CO_FL_WAIT_L4_CONN|CO_FL_WAIT_L6_CONN))) {
		si_conn_send(conn);
		if (conn->flags & CO_FL_ERROR) {
			/* Write error on the file descriptor */
			__conn_data_stop_both(conn);
			si->flags |= SI_FL_ERR;
			goto out_wakeup;
		}
	}

	/* OK, so now we know that some data might have been sent, and that we may
	 * have to poll first. We have to do that too if the buffer is not empty.
	 */
	if (channel_is_empty(ob)) {
		/* the connection is established but we can't write. Either the
		 * buffer is empty, or we just refrain from sending because the
		 * ->o limit was reached. Maybe we just wrote the last
		 * chunk and need to close.
		 */
		__conn_data_stop_send(conn);
		if (((ob->flags & (CF_SHUTW|CF_AUTO_CLOSE|CF_SHUTW_NOW)) ==
		     (CF_AUTO_CLOSE|CF_SHUTW_NOW)) &&
		    (si->state == SI_ST_EST)) {
			si_shutw(si);
			goto out_wakeup;
		}

		if ((ob->flags & (CF_SHUTW|CF_SHUTW_NOW)) == 0)
			si->flags |= SI_FL_WAIT_DATA;
		ob->wex = TICK_ETERNITY;
	}
	else {
		/* Otherwise there are remaining data to be sent in the buffer,
		 * which means we have to poll before doing so.
		 */
		__conn_data_want_send(conn);
		si->flags &= ~SI_FL_WAIT_DATA;
		if (!tick_isset(ob->wex))
			ob->wex = tick_add_ifset(now_ms, ob->wto);
	}

	if (likely(ob->flags & CF_WRITE_ACTIVITY)) {
		/* update timeout if we have written something */
		if ((ob->flags & (CF_SHUTW|CF_WRITE_PARTIAL)) == CF_WRITE_PARTIAL &&
		    !channel_is_empty(ob))
			ob->wex = tick_add_ifset(now_ms, ob->wto);

		if (tick_isset(si->ib->rex) && !(si->flags & SI_FL_INDEP_STR)) {
			/* Note: to prevent the client from expiring read timeouts
			 * during writes, we refresh it. We only do this if the
			 * interface is not configured for "independent streams",
			 * because for some applications it's better not to do this,
			 * for instance when continuously exchanging small amounts
			 * of data which can full the socket buffers long before a
			 * write timeout is detected.
			 */
			si->ib->rex = tick_add_ifset(now_ms, si->ib->rto);
		}
	}

	/* in case of special condition (error, shutdown, end of write...), we
	 * have to notify the task.
	 */
	if (likely((ob->flags & (CF_WRITE_NULL|CF_WRITE_ERROR|CF_SHUTW)) ||
	          ((ob->flags & CF_WAKE_WRITE) &&
	           ((channel_is_empty(si->ob) && !ob->to_forward) ||
	            si->state != SI_ST_EST)))) {
	out_wakeup:
		if (!(si->flags & SI_FL_DONT_WAKE) && si->owner)
			task_wakeup(si->owner, TASK_WOKEN_IO);
	}

	/* commit possible polling changes */
	conn_cond_update_polling(conn);
}
