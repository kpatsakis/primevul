session_subsystem_req(Session *s)
{
	struct stat st;
	u_int len;
	int success = 0;
	char *prog, *cmd;
	u_int i;

	s->subsys = packet_get_string(&len);
	packet_check_eom();
	debug2("subsystem request for %.100s by user %s", s->subsys,
	    s->pw->pw_name);

	for (i = 0; i < options.num_subsystems; i++) {
		if (strcmp(s->subsys, options.subsystem_name[i]) == 0) {
			prog = options.subsystem_command[i];
			cmd = options.subsystem_args[i];
			if (strcmp(INTERNAL_SFTP_NAME, prog) == 0) {
				s->is_subsystem = SUBSYSTEM_INT_SFTP;
				debug("subsystem: %s", prog);
			} else {
				if (stat(prog, &st) < 0)
					debug("subsystem: cannot stat %s: %s",
					    prog, strerror(errno));
				s->is_subsystem = SUBSYSTEM_EXT;
				debug("subsystem: exec() %s", cmd);
			}
			success = do_exec(s, cmd) == 0;
			break;
		}
	}

	if (!success)
		logit("subsystem request for %.100s by user %s failed, "
		    "subsystem not found", s->subsys, s->pw->pw_name);

	return success;
}
