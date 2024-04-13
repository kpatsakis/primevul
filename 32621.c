static void stream_int_shutw_conn(struct stream_interface *si)
{
	struct connection *conn = __objt_conn(si->end);

	si->ob->flags &= ~CF_SHUTW_NOW;
	if (si->ob->flags & CF_SHUTW)
		return;
	si->ob->flags |= CF_SHUTW;
	si->ob->wex = TICK_ETERNITY;
	si->flags &= ~SI_FL_WAIT_DATA;

	switch (si->state) {
	case SI_ST_EST:
		/* we have to shut before closing, otherwise some short messages
		 * may never leave the system, especially when there are remaining
		 * unread data in the socket input buffer, or when nolinger is set.
		 * However, if SI_FL_NOLINGER is explicitly set, we know there is
		 * no risk so we close both sides immediately.
		 */
		if (si->flags & SI_FL_ERR) {
			/* quick close, the socket is alredy shut anyway */
		}
		else if (si->flags & SI_FL_NOLINGER) {
			/* unclean data-layer shutdown */
			if (conn->xprt && conn->xprt->shutw)
				conn->xprt->shutw(conn, 0);
		}
		else {
			/* clean data-layer shutdown */
			if (conn->xprt && conn->xprt->shutw)
				conn->xprt->shutw(conn, 1);

			/* If the stream interface is configured to disable half-open
			 * connections, we'll skip the shutdown(), but only if the
			 * read size is already closed. Otherwise we can't support
			 * closed write with pending read (eg: abortonclose while
			 * waiting for the server).
			 */
			if (!(si->flags & SI_FL_NOHALF) || !(si->ib->flags & (CF_SHUTR|CF_DONT_READ))) {
				/* We shutdown transport layer */
				if (conn_ctrl_ready(conn))
					shutdown(conn->t.sock.fd, SHUT_WR);

				if (!(si->ib->flags & (CF_SHUTR|CF_DONT_READ))) {
					/* OK just a shutw, but we want the caller
					 * to disable polling on this FD if exists.
					 */
					if (conn->ctrl)
						conn_data_stop_send(conn);
					return;
				}
			}
		}

		/* fall through */
	case SI_ST_CON:
		/* we may have to close a pending connection, and mark the
		 * response buffer as shutr
		 */
		conn_full_close(conn);
		/* fall through */
	case SI_ST_CER:
	case SI_ST_QUE:
	case SI_ST_TAR:
		si->state = SI_ST_DIS;
		/* fall through */
	default:
		si->flags &= ~(SI_FL_WAIT_ROOM | SI_FL_NOLINGER);
		si->ib->flags &= ~CF_SHUTR_NOW;
		si->ib->flags |= CF_SHUTR;
		si->ib->rex = TICK_ETERNITY;
		si->exp = TICK_ETERNITY;
	}
}
