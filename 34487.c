session_new(void)
{
	Session *s, *tmp;

	if (sessions_first_unused == -1) {
		if (sessions_nalloc >= options.max_sessions)
			return NULL;
		debug2("%s: allocate (allocated %d max %d)",
		    __func__, sessions_nalloc, options.max_sessions);
		tmp = xreallocarray(sessions, sessions_nalloc + 1,
		    sizeof(*sessions));
		if (tmp == NULL) {
			error("%s: cannot allocate %d sessions",
			    __func__, sessions_nalloc + 1);
			return NULL;
		}
		sessions = tmp;
		session_unused(sessions_nalloc++);
	}

	if (sessions_first_unused >= sessions_nalloc ||
	    sessions_first_unused < 0) {
		fatal("%s: insane first_unused %d max %d nalloc %d",
		    __func__, sessions_first_unused, options.max_sessions,
		    sessions_nalloc);
	}

	s = &sessions[sessions_first_unused];
	if (s->used) {
		fatal("%s: session %d already used",
		    __func__, sessions_first_unused);
	}
	sessions_first_unused = s->next_unused;
	s->used = 1;
	s->next_unused = -1;
	debug("session_new: session %d", s->self);

	return s;
}
