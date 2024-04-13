do_exec(Session *s, const char *command)
{
	struct ssh *ssh = active_state; /* XXX */
	int ret;
	const char *forced = NULL, *tty = NULL;
	char session_type[1024];

	if (options.adm_forced_command) {
		original_command = command;
		command = options.adm_forced_command;
		forced = "(config)";
	} else if (forced_command) {
		original_command = command;
		command = forced_command;
		forced = "(key-option)";
	}
	if (forced != NULL) {
		if (IS_INTERNAL_SFTP(command)) {
			s->is_subsystem = s->is_subsystem ?
			    SUBSYSTEM_INT_SFTP : SUBSYSTEM_INT_SFTP_ERROR;
		} else if (s->is_subsystem)
			s->is_subsystem = SUBSYSTEM_EXT;
		snprintf(session_type, sizeof(session_type),
		    "forced-command %s '%.900s'", forced, command);
	} else if (s->is_subsystem) {
		snprintf(session_type, sizeof(session_type),
		    "subsystem '%.900s'", s->subsys);
	} else if (command == NULL) {
		snprintf(session_type, sizeof(session_type), "shell");
	} else {
		/* NB. we don't log unforced commands to preserve privacy */
		snprintf(session_type, sizeof(session_type), "command");
	}

	if (s->ttyfd != -1) {
		tty = s->tty;
		if (strncmp(tty, "/dev/", 5) == 0)
			tty += 5;
	}

	verbose("Starting session: %s%s%s for %s from %.200s port %d id %d",
	    session_type,
	    tty == NULL ? "" : " on ",
	    tty == NULL ? "" : tty,
	    s->pw->pw_name,
	    ssh_remote_ipaddr(ssh),
	    ssh_remote_port(ssh),
	    s->self);

#ifdef SSH_AUDIT_EVENTS
	if (command != NULL)
		PRIVSEP(audit_run_command(command));
	else if (s->ttyfd == -1) {
		char *shell = s->pw->pw_shell;

		if (shell[0] == '\0')	/* empty shell means /bin/sh */
			shell =_PATH_BSHELL;
		PRIVSEP(audit_run_command(shell));
	}
#endif
	if (s->ttyfd != -1)
		ret = do_exec_pty(s, command);
	else
		ret = do_exec_no_pty(s, command);

	original_command = NULL;

	/*
	 * Clear loginmsg: it's the child's responsibility to display
	 * it to the user, otherwise multiple sessions may accumulate
	 * multiple copies of the login messages.
	 */
	buffer_clear(&loginmsg);

	return ret;
}
