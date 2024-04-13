int posix_cpu_nsleep(const clockid_t which_clock, int flags,
		     struct timespec *rqtp, struct timespec __user *rmtp)
{
	struct restart_block *restart_block =
	    &current_thread_info()->restart_block;
	struct itimerspec it;
	int error;

	/*
	 * Diagnose required errors first.
	 */
	if (CPUCLOCK_PERTHREAD(which_clock) &&
	    (CPUCLOCK_PID(which_clock) == 0 ||
	     CPUCLOCK_PID(which_clock) == current->pid))
		return -EINVAL;

	error = do_cpu_nanosleep(which_clock, flags, rqtp, &it);

	if (error == -ERESTART_RESTARTBLOCK) {

	       	if (flags & TIMER_ABSTIME)
			return -ERESTARTNOHAND;
		/*
	 	 * Report back to the user the time still remaining.
	 	 */
		if (rmtp != NULL && copy_to_user(rmtp, &it.it_value, sizeof *rmtp))
			return -EFAULT;

		restart_block->fn = posix_cpu_nsleep_restart;
		restart_block->arg0 = which_clock;
		restart_block->arg1 = (unsigned long) rmtp;
		restart_block->arg2 = rqtp->tv_sec;
		restart_block->arg3 = rqtp->tv_nsec;
	}
	return error;
}
