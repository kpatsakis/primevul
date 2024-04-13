static void si_idle_conn_null_cb(struct connection *conn)
{
	if (conn->flags & (CO_FL_ERROR | CO_FL_SOCK_RD_SH))
		return;

	if (fdtab[conn->t.sock.fd].ev & (FD_POLL_ERR|FD_POLL_HUP)) {
		fdtab[conn->t.sock.fd].linger_risk = 0;
		conn->flags |= CO_FL_SOCK_RD_SH;
	}
	else {
		conn_drain(conn);
	}

	/* disable draining if we were called and have no drain function */
	if (!conn->ctrl->drain)
		__conn_data_stop_recv(conn);
}
