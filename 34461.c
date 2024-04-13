do_exec_pty(Session *s, const char *command)
{
	int fdout, ptyfd, ttyfd, ptymaster;
	pid_t pid;

	if (s == NULL)
		fatal("do_exec_pty: no session");
	ptyfd = s->ptyfd;
	ttyfd = s->ttyfd;

	/*
	 * Create another descriptor of the pty master side for use as the
	 * standard input.  We could use the original descriptor, but this
	 * simplifies code in server_loop.  The descriptor is bidirectional.
	 * Do this before forking (and cleanup in the child) so as to
	 * detect and gracefully fail out-of-fd conditions.
	 */
	if ((fdout = dup(ptyfd)) < 0) {
		error("%s: dup #1: %s", __func__, strerror(errno));
		close(ttyfd);
		close(ptyfd);
		return -1;
	}
	/* we keep a reference to the pty master */
	if ((ptymaster = dup(ptyfd)) < 0) {
		error("%s: dup #2: %s", __func__, strerror(errno));
		close(ttyfd);
		close(ptyfd);
		close(fdout);
		return -1;
	}

	/* Fork the child. */
	switch ((pid = fork())) {
	case -1:
		error("%s: fork: %.100s", __func__, strerror(errno));
		close(fdout);
		close(ptymaster);
		close(ttyfd);
		close(ptyfd);
		return -1;
	case 0:
		is_child = 1;

		close(fdout);
		close(ptymaster);

		/* Child.  Reinitialize the log because the pid has changed. */
		log_init(__progname, options.log_level,
		    options.log_facility, log_stderr);
		/* Close the master side of the pseudo tty. */
		close(ptyfd);

		/* Make the pseudo tty our controlling tty. */
		pty_make_controlling_tty(&ttyfd, s->tty);

		/* Redirect stdin/stdout/stderr from the pseudo tty. */
		if (dup2(ttyfd, 0) < 0)
			error("dup2 stdin: %s", strerror(errno));
		if (dup2(ttyfd, 1) < 0)
			error("dup2 stdout: %s", strerror(errno));
		if (dup2(ttyfd, 2) < 0)
			error("dup2 stderr: %s", strerror(errno));

		/* Close the extra descriptor for the pseudo tty. */
		close(ttyfd);

		/* record login, etc. similar to login(1) */
#ifndef HAVE_OSF_SIA
		if (!(options.use_login && command == NULL)) {
#ifdef _UNICOS
			cray_init_job(s->pw); /* set up cray jid and tmpdir */
#endif /* _UNICOS */
			do_login(s, command);
		}
# ifdef LOGIN_NEEDS_UTMPX
		else
			do_pre_login(s);
# endif
#endif
		/*
		 * Do common processing for the child, such as execing
		 * the command.
		 */
		do_child(s, command);
		/* NOTREACHED */
	default:
		break;
	}

#ifdef _UNICOS
	signal(WJSIGNAL, cray_job_termination_handler);
#endif /* _UNICOS */
#ifdef HAVE_CYGWIN
	cygwin_set_impersonation_token(INVALID_HANDLE_VALUE);
#endif

	s->pid = pid;

	/* Parent.  Close the slave side of the pseudo tty. */
	close(ttyfd);

	/* Enter interactive session. */
	s->ptymaster = ptymaster;
	packet_set_interactive(1, 
	    options.ip_qos_interactive, options.ip_qos_bulk);
	if (compat20) {
		session_set_fds(s, ptyfd, fdout, -1, 1, 1);
	} else {
		server_loop(pid, ptyfd, fdout, -1);
		/* server_loop _has_ closed ptyfd and fdout. */
	}
	return 0;
}
