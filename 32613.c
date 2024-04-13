static void stream_int_chk_rcv_conn(struct stream_interface *si)
{
	struct channel *ib = si->ib;
	struct connection *conn = __objt_conn(si->end);

	if (unlikely(si->state > SI_ST_EST || (ib->flags & CF_SHUTR)))
		return;

	conn_refresh_polling_flags(conn);

	if ((ib->flags & CF_DONT_READ) || channel_full(ib)) {
		/* stop reading */
		if (!(ib->flags & CF_DONT_READ)) /* full */
			si->flags |= SI_FL_WAIT_ROOM;
		__conn_data_stop_recv(conn);
	}
	else {
		/* (re)start reading */
		si->flags &= ~SI_FL_WAIT_ROOM;
		__conn_data_want_recv(conn);
	}
	conn_cond_update_data_polling(conn);
}
