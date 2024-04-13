static void arm_timer(struct k_itimer *timer, union cpu_time_count now)
{
	struct task_struct *p = timer->it.cpu.task;
	struct list_head *head, *listpos;
	struct cpu_timer_list *const nt = &timer->it.cpu;
	struct cpu_timer_list *next;
	unsigned long i;

	head = (CPUCLOCK_PERTHREAD(timer->it_clock) ?
		p->cpu_timers : p->signal->cpu_timers);
	head += CPUCLOCK_WHICH(timer->it_clock);

	BUG_ON(!irqs_disabled());
	spin_lock(&p->sighand->siglock);

	listpos = head;
	if (CPUCLOCK_WHICH(timer->it_clock) == CPUCLOCK_SCHED) {
		list_for_each_entry(next, head, entry) {
			if (next->expires.sched > nt->expires.sched)
				break;
			listpos = &next->entry;
		}
	} else {
		list_for_each_entry(next, head, entry) {
			if (cputime_gt(next->expires.cpu, nt->expires.cpu))
				break;
			listpos = &next->entry;
		}
	}
	list_add(&nt->entry, listpos);

	if (listpos == head) {
		/*
		 * We are the new earliest-expiring timer.
		 * If we are a thread timer, there can always
		 * be a process timer telling us to stop earlier.
		 */

		if (CPUCLOCK_PERTHREAD(timer->it_clock)) {
			switch (CPUCLOCK_WHICH(timer->it_clock)) {
			default:
				BUG();
			case CPUCLOCK_PROF:
				if (cputime_eq(p->it_prof_expires,
					       cputime_zero) ||
				    cputime_gt(p->it_prof_expires,
					       nt->expires.cpu))
					p->it_prof_expires = nt->expires.cpu;
				break;
			case CPUCLOCK_VIRT:
				if (cputime_eq(p->it_virt_expires,
					       cputime_zero) ||
				    cputime_gt(p->it_virt_expires,
					       nt->expires.cpu))
					p->it_virt_expires = nt->expires.cpu;
				break;
			case CPUCLOCK_SCHED:
				if (p->it_sched_expires == 0 ||
				    p->it_sched_expires > nt->expires.sched)
					p->it_sched_expires = nt->expires.sched;
				break;
			}
		} else {
			/*
			 * For a process timer, we must balance
			 * all the live threads' expirations.
			 */
			switch (CPUCLOCK_WHICH(timer->it_clock)) {
			default:
				BUG();
			case CPUCLOCK_VIRT:
				if (!cputime_eq(p->signal->it_virt_expires,
						cputime_zero) &&
				    cputime_lt(p->signal->it_virt_expires,
					       timer->it.cpu.expires.cpu))
					break;
				goto rebalance;
			case CPUCLOCK_PROF:
				if (!cputime_eq(p->signal->it_prof_expires,
						cputime_zero) &&
				    cputime_lt(p->signal->it_prof_expires,
					       timer->it.cpu.expires.cpu))
					break;
				i = p->signal->rlim[RLIMIT_CPU].rlim_cur;
				if (i != RLIM_INFINITY &&
				    i <= cputime_to_secs(timer->it.cpu.expires.cpu))
					break;
				goto rebalance;
			case CPUCLOCK_SCHED:
			rebalance:
				process_timer_rebalance(
					timer->it.cpu.task,
					CPUCLOCK_WHICH(timer->it_clock),
					timer->it.cpu.expires, now);
				break;
			}
		}
	}

	spin_unlock(&p->sighand->siglock);
}
