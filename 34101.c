Popen(char *command, char *type)
{
    struct pid *cur;
    FILE *iop;
    int pdes[2], pid;

    if (command == NULL || type == NULL)
	return NULL;

    if ((*type != 'r' && *type != 'w') || type[1])
	return NULL;

    if ((cur = malloc(sizeof(struct pid))) == NULL)
	return NULL;

    if (pipe(pdes) < 0) {
	free(cur);
	return NULL;
    }

    /* Ignore the smart scheduler while this is going on */
    old_alarm = OsSignal(SIGALRM, SIG_IGN);
    if (old_alarm == SIG_ERR) {
      close(pdes[0]);
      close(pdes[1]);
      free(cur);
      perror("signal");
      return NULL;
    }

    switch (pid = fork()) {
    case -1: 	/* error */
	close(pdes[0]);
	close(pdes[1]);
	free(cur);
	if (OsSignal(SIGALRM, old_alarm) == SIG_ERR)
	  perror("signal");
	return NULL;
    case 0:	/* child */
	if (setgid(getgid()) == -1)
	    _exit(127);
	if (setuid(getuid()) == -1)
	    _exit(127);
	if (*type == 'r') {
	    if (pdes[1] != 1) {
		/* stdout */
		dup2(pdes[1], 1);
		close(pdes[1]);
	    }
	    close(pdes[0]);
	} else {
	    if (pdes[0] != 0) {
		/* stdin */
		dup2(pdes[0], 0);
		close(pdes[0]);
	    }
	    close(pdes[1]);
	}
	execl("/bin/sh", "sh", "-c", command, (char *)NULL);
	_exit(127);
    }

    /* Avoid EINTR during stdio calls */
    OsBlockSignals ();
    
    /* parent */
    if (*type == 'r') {
	iop = fdopen(pdes[0], type);
	close(pdes[1]);
    } else {
	iop = fdopen(pdes[1], type);
	close(pdes[0]);
    }

    cur->fp = iop;
    cur->pid = pid;
    cur->next = pidlist;
    pidlist = cur;

    DebugF("Popen: `%s', fp = %p\n", command, iop);

    return iop;
}
