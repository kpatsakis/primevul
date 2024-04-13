long posix_cpu_nsleep_restart(struct restart_block *restart_block)
{
	clockid_t which_clock = restart_block->arg0;
	struct timespec __user *rmtp;
	struct timespec t;
	struct itimerspec it;
	int error;

	rmtp = (struct timespec __user *) restart_block->arg1;
	t.tv_sec = restart_block->arg2;
	t.tv_nsec = restart_block->arg3;

	restart_block->fn = do_no_restart_syscall;
	error = do_cpu_nanosleep(which_clock, TIMER_ABSTIME, &t, &it);

	if (error == -ERESTART_RESTARTBLOCK) {
		/*
	 	 * Report back to the user the time still remaining.
	 	 */
		if (rmtp != NULL && copy_to_user(rmtp, &it.it_value, sizeof *rmtp))
			return -EFAULT;

		restart_block->fn = posix_cpu_nsleep_restart;
		restart_block->arg0 = which_clock;
		restart_block->arg1 = (unsigned long) rmtp;
		restart_block->arg2 = t.tv_sec;
		restart_block->arg3 = t.tv_nsec;
	}
	return error;

}
