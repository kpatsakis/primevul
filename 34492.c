session_shell_req(Session *s)
{
	packet_check_eom();
	return do_exec(s, NULL) == 0;
}
