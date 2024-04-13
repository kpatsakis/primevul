static int mmtimer_setup(int cpu, int comparator, unsigned long expires)
{

	switch (comparator) {
	case 0:
		mmtimer_setup_int_0(cpu, expires);
		break;
	case 1:
		mmtimer_setup_int_1(cpu, expires);
		break;
	case 2:
		mmtimer_setup_int_2(cpu, expires);
		break;
	}
	/* We might've missed our expiration time */
	if (rtc_time() <= expires)
		return 1;

	/*
	 * If an interrupt is already pending then its okay
	 * if not then we failed
	 */
	return mmtimer_int_pending(comparator);
}
