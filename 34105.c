System(char *command)
{
    int pid, p;
    void (*csig)(int);
    int status;

    if (!command)
	return 1;

    csig = signal(SIGCHLD, SIG_DFL);
    if (csig == SIG_ERR) {
      perror("signal");
      return -1;
    }
    DebugF("System: `%s'\n", command);

    switch (pid = fork()) {
    case -1:	/* error */
	p = -1;
    case 0:	/* child */
	if (setgid(getgid()) == -1)
	    _exit(127);
	if (setuid(getuid()) == -1)
	    _exit(127);
	execl("/bin/sh", "sh", "-c", command, (char *)NULL);
	_exit(127);
    default:	/* parent */
	do {
	    p = waitpid(pid, &status, 0);
	} while (p == -1 && errno == EINTR);
	
    }

    if (signal(SIGCHLD, csig) == SIG_ERR) {
      perror("signal");
      return -1;
    }

    return p == -1 ? -1 : status;
}
