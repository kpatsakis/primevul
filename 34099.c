OsSignal(int sig, OsSigHandlerPtr handler)
{
    struct sigaction act, oact;

    sigemptyset(&act.sa_mask);
    if (handler != SIG_IGN)
	sigaddset(&act.sa_mask, sig);
    act.sa_flags = 0;
    act.sa_handler = handler;
    if (sigaction(sig, &act, &oact))
      perror("sigaction");
    return oact.sa_handler;
}
