static __init int init_posix_cpu_timers(void)
{
	struct k_clock process = {
		.clock_getres = process_cpu_clock_getres,
		.clock_get = process_cpu_clock_get,
		.clock_set = do_posix_clock_nosettime,
		.timer_create = process_cpu_timer_create,
		.nsleep = process_cpu_nsleep,
		.nsleep_restart = process_cpu_nsleep_restart,
	};
	struct k_clock thread = {
		.clock_getres = thread_cpu_clock_getres,
		.clock_get = thread_cpu_clock_get,
		.clock_set = do_posix_clock_nosettime,
		.timer_create = thread_cpu_timer_create,
		.nsleep = thread_cpu_nsleep,
		.nsleep_restart = thread_cpu_nsleep_restart,
	};

	register_posix_clock(CLOCK_PROCESS_CPUTIME_ID, &process);
	register_posix_clock(CLOCK_THREAD_CPUTIME_ID, &thread);

	return 0;
}
