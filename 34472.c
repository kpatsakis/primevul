session_break_req(Session *s)
{

	packet_get_int();	/* ignored */
	packet_check_eom();

	if (s->ptymaster == -1 || tcsendbreak(s->ptymaster, 0) < 0)
		return 0;
	return 1;
}
