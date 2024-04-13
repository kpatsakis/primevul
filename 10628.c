inbound_exec_eom_cmd (char *str, void *sess)
{
	char *cmd;

	cmd = command_insert_vars ((session*)sess, (str[0] == '/') ? str + 1 : str);
	handle_command ((session*)sess, cmd, TRUE);
	g_free (cmd);

	return 1;
}