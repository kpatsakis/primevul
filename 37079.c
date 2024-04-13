static int process_cpu_clock_get(const clockid_t which_clock,
				 struct timespec *tp)
{
	return posix_cpu_clock_get(PROCESS_CLOCK, tp);
}
