static int si_conn_wake_cb(struct connection *conn)
{
	struct stream_interface *si = conn->owner;

	DPRINTF(stderr, "%s: si=%p, si->state=%d ib->flags=%08x ob->flags=%08x\n",
		__FUNCTION__,
		si, si->state, si->ib->flags, si->ob->flags);

	if (conn->flags & CO_FL_ERROR)
		si->flags |= SI_FL_ERR;

	/* check for recent connection establishment */
	if (unlikely(!(conn->flags & (CO_FL_WAIT_L4_CONN | CO_FL_WAIT_L6_CONN | CO_FL_CONNECTED)))) {
		si->exp = TICK_ETERNITY;
		si->ob->flags |= CF_WRITE_NULL;
	}

	/* process consumer side */
	if (channel_is_empty(si->ob)) {
		if (((si->ob->flags & (CF_SHUTW|CF_SHUTW_NOW)) == CF_SHUTW_NOW) &&
		    (si->state == SI_ST_EST))
			stream_int_shutw_conn(si);
		__conn_data_stop_send(conn);
		si->ob->wex = TICK_ETERNITY;
	}

	if ((si->ob->flags & (CF_SHUTW|CF_SHUTW_NOW)) == 0 && !channel_full(si->ob))
		si->flags |= SI_FL_WAIT_DATA;

	if (si->ob->flags & CF_WRITE_ACTIVITY) {
		/* update timeouts if we have written something */
		if ((si->ob->flags & (CF_SHUTW|CF_WRITE_PARTIAL)) == CF_WRITE_PARTIAL &&
		    !channel_is_empty(si->ob))
			if (tick_isset(si->ob->wex))
				si->ob->wex = tick_add_ifset(now_ms, si->ob->wto);

		if (!(si->flags & SI_FL_INDEP_STR))
			if (tick_isset(si->ib->rex))
				si->ib->rex = tick_add_ifset(now_ms, si->ib->rto);

		if (likely((si->ob->flags & (CF_SHUTW|CF_WRITE_PARTIAL|CF_DONT_READ)) == CF_WRITE_PARTIAL &&
			   !channel_full(si->ob) &&
			   (si->ob->prod->flags & SI_FL_WAIT_ROOM)))
			si_chk_rcv(si->ob->prod);
	}

	/* process producer side.
	 * We might have some data the consumer is waiting for.
	 * We can do fast-forwarding, but we avoid doing this for partial
	 * buffers, because it is very likely that it will be done again
	 * immediately afterwards once the following data is parsed (eg:
	 * HTTP chunking).
	 */
	if (((si->ib->flags & CF_READ_PARTIAL) && !channel_is_empty(si->ib)) &&
	    (si->ib->pipe /* always try to send spliced data */ ||
	     (si->ib->buf->i == 0 && (si->ib->cons->flags & SI_FL_WAIT_DATA)))) {
		int last_len = si->ib->pipe ? si->ib->pipe->data : 0;

		si_chk_snd(si->ib->cons);

		/* check if the consumer has freed some space either in the
		 * buffer or in the pipe.
		 */
		if (!channel_full(si->ib) &&
		    (!last_len || !si->ib->pipe || si->ib->pipe->data < last_len))
			si->flags &= ~SI_FL_WAIT_ROOM;
	}

	if (si->flags & SI_FL_WAIT_ROOM) {
		__conn_data_stop_recv(conn);
		si->ib->rex = TICK_ETERNITY;
	}
	else if ((si->ib->flags & (CF_SHUTR|CF_READ_PARTIAL|CF_DONT_READ)) == CF_READ_PARTIAL &&
		 !channel_full(si->ib)) {
		/* we must re-enable reading if si_chk_snd() has freed some space */
		__conn_data_want_recv(conn);
		if (!(si->ib->flags & CF_READ_NOEXP) && tick_isset(si->ib->rex))
			si->ib->rex = tick_add_ifset(now_ms, si->ib->rto);
	}

	/* wake the task up only when needed */
	if (/* changes on the production side */
	    (si->ib->flags & (CF_READ_NULL|CF_READ_ERROR)) ||
	    si->state != SI_ST_EST ||
	    (si->flags & SI_FL_ERR) ||
	    ((si->ib->flags & CF_READ_PARTIAL) &&
	     (!si->ib->to_forward || si->ib->cons->state != SI_ST_EST)) ||

	    /* changes on the consumption side */
	    (si->ob->flags & (CF_WRITE_NULL|CF_WRITE_ERROR)) ||
	    ((si->ob->flags & CF_WRITE_ACTIVITY) &&
	     ((si->ob->flags & CF_SHUTW) ||
	      ((si->ob->flags & CF_WAKE_WRITE) &&
	       (si->ob->prod->state != SI_ST_EST ||
	        (channel_is_empty(si->ob) && !si->ob->to_forward)))))) {
		task_wakeup(si->owner, TASK_WOKEN_IO);
	}
	if (si->ib->flags & CF_READ_ACTIVITY)
		si->ib->flags &= ~CF_READ_DONTWAIT;
	return 0;
}
