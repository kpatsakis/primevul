static void check_process_timers(struct task_struct *tsk,
				 struct list_head *firing)
{
	int maxfire;
	struct signal_struct *const sig = tsk->signal;
	cputime_t utime, stime, ptime, virt_expires, prof_expires;
	unsigned long long sum_sched_runtime, sched_expires;
	struct task_struct *t;
	struct list_head *timers = sig->cpu_timers;

	/*
	 * Don't sample the current process CPU clocks if there are no timers.
	 */
	if (list_empty(&timers[CPUCLOCK_PROF]) &&
	    cputime_eq(sig->it_prof_expires, cputime_zero) &&
	    sig->rlim[RLIMIT_CPU].rlim_cur == RLIM_INFINITY &&
	    list_empty(&timers[CPUCLOCK_VIRT]) &&
	    cputime_eq(sig->it_virt_expires, cputime_zero) &&
	    list_empty(&timers[CPUCLOCK_SCHED]))
		return;

	/*
	 * Collect the current process totals.
	 */
	utime = sig->utime;
	stime = sig->stime;
	sum_sched_runtime = sig->sum_sched_runtime;
	t = tsk;
	do {
		utime = cputime_add(utime, t->utime);
		stime = cputime_add(stime, t->stime);
		sum_sched_runtime += t->se.sum_exec_runtime;
		t = next_thread(t);
	} while (t != tsk);
	ptime = cputime_add(utime, stime);

	maxfire = 20;
	prof_expires = cputime_zero;
	while (!list_empty(timers)) {
		struct cpu_timer_list *tl = list_first_entry(timers,
						      struct cpu_timer_list,
						      entry);
		if (!--maxfire || cputime_lt(ptime, tl->expires.cpu)) {
			prof_expires = tl->expires.cpu;
			break;
		}
		tl->firing = 1;
		list_move_tail(&tl->entry, firing);
	}

	++timers;
	maxfire = 20;
	virt_expires = cputime_zero;
	while (!list_empty(timers)) {
		struct cpu_timer_list *tl = list_first_entry(timers,
						      struct cpu_timer_list,
						      entry);
		if (!--maxfire || cputime_lt(utime, tl->expires.cpu)) {
			virt_expires = tl->expires.cpu;
			break;
		}
		tl->firing = 1;
		list_move_tail(&tl->entry, firing);
	}

	++timers;
	maxfire = 20;
	sched_expires = 0;
	while (!list_empty(timers)) {
		struct cpu_timer_list *tl = list_first_entry(timers,
						      struct cpu_timer_list,
						      entry);
		if (!--maxfire || sum_sched_runtime < tl->expires.sched) {
			sched_expires = tl->expires.sched;
			break;
		}
		tl->firing = 1;
		list_move_tail(&tl->entry, firing);
	}

	/*
	 * Check for the special case process timers.
	 */
	if (!cputime_eq(sig->it_prof_expires, cputime_zero)) {
		if (cputime_ge(ptime, sig->it_prof_expires)) {
			/* ITIMER_PROF fires and reloads.  */
			sig->it_prof_expires = sig->it_prof_incr;
			if (!cputime_eq(sig->it_prof_expires, cputime_zero)) {
				sig->it_prof_expires = cputime_add(
					sig->it_prof_expires, ptime);
			}
			__group_send_sig_info(SIGPROF, SEND_SIG_PRIV, tsk);
		}
		if (!cputime_eq(sig->it_prof_expires, cputime_zero) &&
		    (cputime_eq(prof_expires, cputime_zero) ||
		     cputime_lt(sig->it_prof_expires, prof_expires))) {
			prof_expires = sig->it_prof_expires;
		}
	}
	if (!cputime_eq(sig->it_virt_expires, cputime_zero)) {
		if (cputime_ge(utime, sig->it_virt_expires)) {
			/* ITIMER_VIRTUAL fires and reloads.  */
			sig->it_virt_expires = sig->it_virt_incr;
			if (!cputime_eq(sig->it_virt_expires, cputime_zero)) {
				sig->it_virt_expires = cputime_add(
					sig->it_virt_expires, utime);
			}
			__group_send_sig_info(SIGVTALRM, SEND_SIG_PRIV, tsk);
		}
		if (!cputime_eq(sig->it_virt_expires, cputime_zero) &&
		    (cputime_eq(virt_expires, cputime_zero) ||
		     cputime_lt(sig->it_virt_expires, virt_expires))) {
			virt_expires = sig->it_virt_expires;
		}
	}
	if (sig->rlim[RLIMIT_CPU].rlim_cur != RLIM_INFINITY) {
		unsigned long psecs = cputime_to_secs(ptime);
		cputime_t x;
		if (psecs >= sig->rlim[RLIMIT_CPU].rlim_max) {
			/*
			 * At the hard limit, we just die.
			 * No need to calculate anything else now.
			 */
			__group_send_sig_info(SIGKILL, SEND_SIG_PRIV, tsk);
			return;
		}
		if (psecs >= sig->rlim[RLIMIT_CPU].rlim_cur) {
			/*
			 * At the soft limit, send a SIGXCPU every second.
			 */
			__group_send_sig_info(SIGXCPU, SEND_SIG_PRIV, tsk);
			if (sig->rlim[RLIMIT_CPU].rlim_cur
			    < sig->rlim[RLIMIT_CPU].rlim_max) {
				sig->rlim[RLIMIT_CPU].rlim_cur++;
			}
		}
		x = secs_to_cputime(sig->rlim[RLIMIT_CPU].rlim_cur);
		if (cputime_eq(prof_expires, cputime_zero) ||
		    cputime_lt(x, prof_expires)) {
			prof_expires = x;
		}
	}

	if (!cputime_eq(prof_expires, cputime_zero) ||
	    !cputime_eq(virt_expires, cputime_zero) ||
	    sched_expires != 0) {
		/*
		 * Rebalance the threads' expiry times for the remaining
		 * process CPU timers.
		 */

		cputime_t prof_left, virt_left, ticks;
		unsigned long long sched_left, sched;
		const unsigned int nthreads = atomic_read(&sig->live);

		if (!nthreads)
			return;

		prof_left = cputime_sub(prof_expires, utime);
		prof_left = cputime_sub(prof_left, stime);
		prof_left = cputime_div_non_zero(prof_left, nthreads);
		virt_left = cputime_sub(virt_expires, utime);
		virt_left = cputime_div_non_zero(virt_left, nthreads);
		if (sched_expires) {
			sched_left = sched_expires - sum_sched_runtime;
			do_div(sched_left, nthreads);
			sched_left = max_t(unsigned long long, sched_left, 1);
		} else {
			sched_left = 0;
		}
		t = tsk;
		do {
			if (unlikely(t->flags & PF_EXITING))
				continue;

			ticks = cputime_add(cputime_add(t->utime, t->stime),
					    prof_left);
			if (!cputime_eq(prof_expires, cputime_zero) &&
			    (cputime_eq(t->it_prof_expires, cputime_zero) ||
			     cputime_gt(t->it_prof_expires, ticks))) {
				t->it_prof_expires = ticks;
			}

			ticks = cputime_add(t->utime, virt_left);
			if (!cputime_eq(virt_expires, cputime_zero) &&
			    (cputime_eq(t->it_virt_expires, cputime_zero) ||
			     cputime_gt(t->it_virt_expires, ticks))) {
				t->it_virt_expires = ticks;
			}

			sched = t->se.sum_exec_runtime + sched_left;
			if (sched_expires && (t->it_sched_expires == 0 ||
					      t->it_sched_expires > sched)) {
				t->it_sched_expires = sched;
			}
		} while ((t = next_thread(t)) != tsk);
	}
}
