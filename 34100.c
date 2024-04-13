Pclose(pointer iop)
{
    struct pid *cur, *last;
    int pstat;
    int pid;

    DebugF("Pclose: fp = %p\n", iop);
    fclose(iop);

    for (last = NULL, cur = pidlist; cur; last = cur, cur = cur->next)
	if (cur->fp == iop)
	    break;
    if (cur == NULL)
	return -1;

    do {
	pid = waitpid(cur->pid, &pstat, 0);
    } while (pid == -1 && errno == EINTR);

    if (last == NULL)
	pidlist = cur->next;
    else
	last->next = cur->next;
    free(cur);

    /* allow EINTR again */
    OsReleaseSignals ();
    
    if (old_alarm && OsSignal(SIGALRM, old_alarm) == SIG_ERR) {
      perror("signal");
      return -1;
    }

    return pid == -1 ? -1 : pstat;
}
