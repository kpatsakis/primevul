void set_process_cpu_timer(struct task_struct *tsk, unsigned int clock_idx,
			   cputime_t *newval, cputime_t *oldval)
{
	union cpu_time_count now;
	struct list_head *head;

	BUG_ON(clock_idx == CPUCLOCK_SCHED);
	cpu_clock_sample_group_locked(clock_idx, tsk, &now);

	if (oldval) {
		if (!cputime_eq(*oldval, cputime_zero)) {
			if (cputime_le(*oldval, now.cpu)) {
				/* Just about to fire. */
				*oldval = jiffies_to_cputime(1);
			} else {
				*oldval = cputime_sub(*oldval, now.cpu);
			}
		}

		if (cputime_eq(*newval, cputime_zero))
			return;
		*newval = cputime_add(*newval, now.cpu);

		/*
		 * If the RLIMIT_CPU timer will expire before the
		 * ITIMER_PROF timer, we have nothing else to do.
		 */
		if (tsk->signal->rlim[RLIMIT_CPU].rlim_cur
		    < cputime_to_secs(*newval))
			return;
	}

	/*
	 * Check whether there are any process timers already set to fire
	 * before this one.  If so, we don't have anything more to do.
	 */
	head = &tsk->signal->cpu_timers[clock_idx];
	if (list_empty(head) ||
	    cputime_ge(list_first_entry(head,
				  struct cpu_timer_list, entry)->expires.cpu,
		       *newval)) {
		/*
		 * Rejigger each thread's expiry time so that one will
		 * notice before we hit the process-cumulative expiry time.
		 */
		union cpu_time_count expires = { .sched = 0 };
		expires.cpu = *newval;
		process_timer_rebalance(tsk, clock_idx, expires, now);
	}
}
