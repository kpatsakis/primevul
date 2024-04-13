OsBlockSignals (void)
{
#ifdef SIG_BLOCK
    if (BlockedSignalCount++ == 0)
    {
	sigset_t    set;
	
	sigemptyset (&set);
	sigaddset (&set, SIGALRM);
	sigaddset (&set, SIGVTALRM);
#ifdef SIGWINCH
	sigaddset (&set, SIGWINCH);
#endif
#ifdef SIGIO
	sigaddset (&set, SIGIO);
#endif
	sigaddset (&set, SIGTSTP);
	sigaddset (&set, SIGTTIN);
	sigaddset (&set, SIGTTOU);
	sigaddset (&set, SIGCHLD);
	sigprocmask (SIG_BLOCK, &set, &PreviousSignalMask);
    }
#endif
}
