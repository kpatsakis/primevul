static int process_cpu_nsleep(const clockid_t which_clock, int flags,
			      struct timespec *rqtp,
			      struct timespec __user *rmtp)
{
	return posix_cpu_nsleep(PROCESS_CLOCK, flags, rqtp, rmtp);
}
