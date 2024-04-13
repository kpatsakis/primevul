run_coprocess(pam_handle_t *pamh, const char *input, char **output,
	      uid_t uid, gid_t gid, const char *command, ...)
{
	int ipipe[2], opipe[2], i;
	char buf[LINE_MAX];
	pid_t child;
	char *buffer = NULL;
	size_t buffer_size = 0;
	va_list ap;

	*output = NULL;

	/* Create stdio pipery. */
	if (pipe(ipipe) == -1) {
		return -1;
	}
	if (pipe(opipe) == -1) {
		close(ipipe[0]);
		close(ipipe[1]);
		return -1;
	}

	/* Fork off a child. */
	child = fork();
	if (child == -1) {
		close(ipipe[0]);
		close(ipipe[1]);
		close(opipe[0]);
		close(opipe[1]);
		return -1;
	}

	if (child == 0) {
		/* We're the child. */
		size_t j;
		char *args[10];
		const char *tmp;
		int maxopened;
		/* Drop privileges. */
		if (setgid(gid) == -1)
		  {
		    int err = errno;
		    pam_syslog (pamh, LOG_ERR, "setgid(%lu) failed: %m",
				(unsigned long) getegid ());
		    _exit (err);
		  }
		if (setgroups(0, NULL) == -1)
		  {
		    int err = errno;
		    pam_syslog (pamh, LOG_ERR, "setgroups() failed: %m");
		    _exit (err);
		  }
		if (setuid(uid) == -1)
		  {
		    int err = errno;
		    pam_syslog (pamh, LOG_ERR, "setuid(%lu) failed: %m",
				(unsigned long) geteuid ());
		    _exit (err);
		  }
		/* Initialize the argument list. */
		memset(args, 0, sizeof(args));
		/* Set the pipe descriptors up as stdin and stdout, and close
		 * everything else, including the original values for the
		 * descriptors. */
		dup2(ipipe[0], STDIN_FILENO);
		dup2(opipe[1], STDOUT_FILENO);
		maxopened = (int)sysconf(_SC_OPEN_MAX);
		for (i = 0; i < maxopened; i++) {
			if ((i != STDIN_FILENO) && (i != STDOUT_FILENO)) {
				close(i);
			}
		}
		/* Convert the varargs list into a regular array of strings. */
		va_start(ap, command);
		args[0] = strdup(command);
		for (j = 1; j < ((sizeof(args) / sizeof(args[0])) - 1); j++) {
			tmp = va_arg(ap, const char*);
			if (tmp == NULL) {
				break;
			}
			args[j] = strdup(tmp);
		}
		/* Run the command. */
		execv(command, args);
		/* Never reached. */
		_exit(1);
	}

	/* We're the parent, so close the other ends of the pipes. */
	close(ipipe[0]);
	close(opipe[1]);
	/* Send input to the process (if we have any), then send an EOF. */
	if (input) {
		(void)pam_modutil_write(ipipe[1], input, strlen(input));
	}
	close(ipipe[1]);

	/* Read data output until we run out of stuff to read. */
	i = pam_modutil_read(opipe[0], buf, sizeof(buf));
	while ((i != 0) && (i != -1)) {
		char *tmp;
		/* Resize the buffer to hold the data. */
		tmp = realloc(buffer, buffer_size + i + 1);
		if (tmp == NULL) {
			/* Uh-oh, bail. */
			if (buffer != NULL) {
				free(buffer);
			}
			close(opipe[0]);
			waitpid(child, NULL, 0);
			return -1;
		}
		/* Save the new buffer location, copy the newly-read data into
		 * the buffer, and make sure the result will be
		 * nul-terminated. */
		buffer = tmp;
		memcpy(buffer + buffer_size, buf, i);
		buffer[buffer_size + i] = '\0';
		buffer_size += i;
		/* Try to read again. */
		i = pam_modutil_read(opipe[0], buf, sizeof(buf));
	}
	/* No more data.  Clean up and return data. */
	close(opipe[0]);
	*output = buffer;
	waitpid(child, NULL, 0);
	return 0;
}
