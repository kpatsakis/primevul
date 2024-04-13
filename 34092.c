Fopen(char *file, char *type)
{
    FILE *iop;
#ifndef HAS_SAVED_IDS_AND_SETEUID
    struct pid *cur;
    int pdes[2], pid;

    if (file == NULL || type == NULL)
	return NULL;

    if ((*type != 'r' && *type != 'w') || type[1])
	return NULL;

    if ((cur = malloc(sizeof(struct pid))) == NULL)
	return NULL;

    if (pipe(pdes) < 0) {
	free(cur);
	return NULL;
    }

    switch (pid = fork()) {
    case -1: 	/* error */
	close(pdes[0]);
	close(pdes[1]);
	free(cur);
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
	execl("/bin/cat", "cat", file, (char *)NULL);
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

    DebugF("Fopen(%s), fp = %p\n", file, iop);

    return iop;
#else
    int ruid, euid;

    ruid = getuid();
    euid = geteuid();
    
    if (seteuid(ruid) == -1) {
	    return NULL;
    }
    iop = fopen(file, type);

    if (seteuid(euid) == -1) {
	    fclose(iop);
	    return NULL;
    }
    return iop;
#endif /* HAS_SAVED_IDS_AND_SETEUID */
}
