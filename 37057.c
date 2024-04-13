static void check_thread_timers(struct task_struct *tsk,
				struct list_head *firing)
{
	int maxfire;
	struct list_head *timers = tsk->cpu_timers;
	struct signal_struct *const sig = tsk->signal;

	maxfire = 20;
	tsk->it_prof_expires = cputime_zero;
	while (!list_empty(timers)) {
		struct cpu_timer_list *t = list_first_entry(timers,
						      struct cpu_timer_list,
						      entry);
		if (!--maxfire || cputime_lt(prof_ticks(tsk), t->expires.cpu)) {
			tsk->it_prof_expires = t->expires.cpu;
			break;
		}
		t->firing = 1;
		list_move_tail(&t->entry, firing);
	}

	++timers;
	maxfire = 20;
	tsk->it_virt_expires = cputime_zero;
	while (!list_empty(timers)) {
		struct cpu_timer_list *t = list_first_entry(timers,
						      struct cpu_timer_list,
						      entry);
		if (!--maxfire || cputime_lt(virt_ticks(tsk), t->expires.cpu)) {
			tsk->it_virt_expires = t->expires.cpu;
			break;
		}
		t->firing = 1;
		list_move_tail(&t->entry, firing);
	}

	++timers;
	maxfire = 20;
	tsk->it_sched_expires = 0;
	while (!list_empty(timers)) {
		struct cpu_timer_list *t = list_first_entry(timers,
						      struct cpu_timer_list,
						      entry);
		if (!--maxfire || tsk->se.sum_exec_runtime < t->expires.sched) {
			tsk->it_sched_expires = t->expires.sched;
			break;
		}
		t->firing = 1;
		list_move_tail(&t->entry, firing);
	}

	/*
	 * Check for the special case thread timers.
	 */
	if (sig->rlim[RLIMIT_RTTIME].rlim_cur != RLIM_INFINITY) {
		unsigned long hard = sig->rlim[RLIMIT_RTTIME].rlim_max;
		unsigned long *soft = &sig->rlim[RLIMIT_RTTIME].rlim_cur;

		if (hard != RLIM_INFINITY &&
		    tsk->rt.timeout > DIV_ROUND_UP(hard, USEC_PER_SEC/HZ)) {
			/*
			 * At the hard limit, we just die.
			 * No need to calculate anything else now.
			 */
			__group_send_sig_info(SIGKILL, SEND_SIG_PRIV, tsk);
			return;
		}
		if (tsk->rt.timeout > DIV_ROUND_UP(*soft, USEC_PER_SEC/HZ)) {
			/*
			 * At the soft limit, send a SIGXCPU every second.
			 */
			if (sig->rlim[RLIMIT_RTTIME].rlim_cur
			    < sig->rlim[RLIMIT_RTTIME].rlim_max) {
				sig->rlim[RLIMIT_RTTIME].rlim_cur +=
								USEC_PER_SEC;
			}
			__group_send_sig_info(SIGXCPU, SEND_SIG_PRIV, tsk);
		}
	}
}
