session_dump(void)
{
	int i;
	for (i = 0; i < sessions_nalloc; i++) {
		Session *s = &sessions[i];

		debug("dump: used %d next_unused %d session %d %p "
		    "channel %d pid %ld",
		    s->used,
		    s->next_unused,
		    s->self,
		    s,
		    s->chanid,
		    (long)s->pid);
	}
}
