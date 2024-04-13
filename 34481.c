session_close_x11(int id)
{
	Channel *c;

	if ((c = channel_by_id(id)) == NULL) {
		debug("session_close_x11: x11 channel %d missing", id);
	} else {
		/* Detach X11 listener */
		debug("session_close_x11: detach x11 channel %d", id);
		channel_cancel_cleanup(id);
		if (c->ostate != CHAN_OUTPUT_CLOSED)
			chan_mark_dead(c);
	}
}
