session_close_single_x11(int id, void *arg)
{
	Session *s;
	u_int i;

	debug3("session_close_single_x11: channel %d", id);
	channel_cancel_cleanup(id);
	if ((s = session_by_x11_channel(id)) == NULL)
		fatal("session_close_single_x11: no x11 channel %d", id);
	for (i = 0; s->x11_chanids[i] != -1; i++) {
		debug("session_close_single_x11: session %d: "
		    "closing channel %d", s->self, s->x11_chanids[i]);
		/*
		 * The channel "id" is already closing, but make sure we
		 * close all of its siblings.
		 */
		if (s->x11_chanids[i] != id)
			session_close_x11(s->x11_chanids[i]);
	}
	free(s->x11_chanids);
	s->x11_chanids = NULL;
	free(s->display);
	s->display = NULL;
	free(s->auth_proto);
	s->auth_proto = NULL;
	free(s->auth_data);
	s->auth_data = NULL;
	free(s->auth_display);
	s->auth_display = NULL;
}
