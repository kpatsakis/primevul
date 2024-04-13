void stream_sock_read0(struct stream_interface *si)
{
	struct connection *conn = __objt_conn(si->end);

	si->ib->flags &= ~CF_SHUTR_NOW;
	if (si->ib->flags & CF_SHUTR)
		return;
	si->ib->flags |= CF_SHUTR;
	si->ib->rex = TICK_ETERNITY;
	si->flags &= ~SI_FL_WAIT_ROOM;

	if (si->state != SI_ST_EST && si->state != SI_ST_CON)
		return;

	if (si->ob->flags & CF_SHUTW)
		goto do_close;

	if (si->flags & SI_FL_NOHALF) {
		/* we want to immediately forward this close to the write side */
		/* force flag on ssl to keep session in cache */
		if (conn->xprt->shutw)
			conn->xprt->shutw(conn, 0);
		goto do_close;
	}

	/* otherwise that's just a normal read shutdown */
	if (conn_ctrl_ready(conn))
		fdtab[conn->t.sock.fd].linger_risk = 0;
	__conn_data_stop_recv(conn);
	return;

 do_close:
	/* OK we completely close the socket here just as if we went through si_shut[rw]() */
	conn_full_close(conn);

	si->ib->flags &= ~CF_SHUTR_NOW;
	si->ib->flags |= CF_SHUTR;
	si->ib->rex = TICK_ETERNITY;

	si->ob->flags &= ~CF_SHUTW_NOW;
	si->ob->flags |= CF_SHUTW;
	si->ob->wex = TICK_ETERNITY;

	si->flags &= ~(SI_FL_WAIT_DATA | SI_FL_WAIT_ROOM);

	si->state = SI_ST_DIS;
	si->exp = TICK_ETERNITY;
	return;
}
