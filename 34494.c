session_tty_list(void)
{
	static char buf[1024];
	int i;
	char *cp;

	buf[0] = '\0';
	for (i = 0; i < sessions_nalloc; i++) {
		Session *s = &sessions[i];
		if (s->used && s->ttyfd != -1) {

			if (strncmp(s->tty, "/dev/", 5) != 0) {
				cp = strrchr(s->tty, '/');
				cp = (cp == NULL) ? s->tty : cp + 1;
			} else
				cp = s->tty + 5;

			if (buf[0] != '\0')
				strlcat(buf, ",", sizeof buf);
			strlcat(buf, cp, sizeof buf);
		}
	}
	if (buf[0] == '\0')
		strlcpy(buf, "notty", sizeof buf);
	return buf;
}
