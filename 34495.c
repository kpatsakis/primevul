session_unused(int id)
{
	debug3("%s: session id %d unused", __func__, id);
	if (id >= options.max_sessions ||
	    id >= sessions_nalloc) {
		fatal("%s: insane session id %d (max %d nalloc %d)",
		    __func__, id, options.max_sessions, sessions_nalloc);
	}
	memset(&sessions[id], 0, sizeof(*sessions));
	sessions[id].self = id;
	sessions[id].used = 0;
	sessions[id].chanid = -1;
	sessions[id].ptyfd = -1;
	sessions[id].ttyfd = -1;
	sessions[id].ptymaster = -1;
	sessions[id].x11_chanids = NULL;
	sessions[id].next_unused = sessions_first_unused;
	sessions_first_unused = id;
}
