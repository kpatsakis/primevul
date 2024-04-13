session_exec_req(Session *s)
{
	u_int len, success;

	char *command = packet_get_string(&len);
	packet_check_eom();
	success = do_exec(s, command) == 0;
	free(command);
	return success;
}
