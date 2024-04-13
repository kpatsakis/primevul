static int sgi_timer_create(struct k_itimer *timer)
{
	/* Insure that a newly created timer is off */
	timer->it.mmtimer.clock = TIMER_OFF;
	return 0;
}
