static int thread_cpu_clock_getres(const clockid_t which_clock,
				   struct timespec *tp)
{
	return posix_cpu_clock_getres(THREAD_CLOCK, tp);
}
