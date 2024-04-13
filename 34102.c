SmartScheduleInit (void)
{
    struct sigaction	act;

    if (SmartScheduleDisable)
	return;

    memset((char *) &act, 0, sizeof(struct sigaction));

    /* Set up the timer signal function */
    act.sa_handler = SmartScheduleTimer;
    sigemptyset (&act.sa_mask);
    sigaddset (&act.sa_mask, SIGALRM);
    if (sigaction (SIGALRM, &act, 0) < 0)
    {
	perror ("sigaction for smart scheduler");
	SmartScheduleDisable = TRUE;
    }
}
