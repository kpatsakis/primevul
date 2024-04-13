SmartScheduleStopTimer (void)
{
    struct itimerval	timer;
    
    if (SmartScheduleDisable)
	return;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    (void) setitimer (ITIMER_REAL, &timer, 0);
}
