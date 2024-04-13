static void si_conn_send_cb(struct connection *conn)
{
	struct stream_interface *si = conn->owner;
	struct channel *chn = si->ob;

	if (conn->flags & CO_FL_ERROR)
		return;

	if (conn->flags & CO_FL_HANDSHAKE)
		/* a handshake was requested */
		return;

	/* we might have been called just after an asynchronous shutw */
	if (chn->flags & CF_SHUTW)
		return;

	/* OK there are data waiting to be sent */
	si_conn_send(conn);

	/* OK all done */
	return;
}
