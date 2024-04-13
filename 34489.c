session_pty_cleanup(Session *s)
{
	PRIVSEP(session_pty_cleanup2(s));
}
