static void process_timer_rebalance(struct task_struct *p,
				    unsigned int clock_idx,
				    union cpu_time_count expires,
				    union cpu_time_count val)
{
	cputime_t ticks, left;
	unsigned long long ns, nsleft;
 	struct task_struct *t = p;
	unsigned int nthreads = atomic_read(&p->signal->live);

	if (!nthreads)
		return;

	switch (clock_idx) {
	default:
		BUG();
		break;
	case CPUCLOCK_PROF:
		left = cputime_div_non_zero(cputime_sub(expires.cpu, val.cpu),
				       nthreads);
		do {
			if (likely(!(t->flags & PF_EXITING))) {
				ticks = cputime_add(prof_ticks(t), left);
				if (cputime_eq(t->it_prof_expires,
					       cputime_zero) ||
				    cputime_gt(t->it_prof_expires, ticks)) {
					t->it_prof_expires = ticks;
				}
			}
			t = next_thread(t);
		} while (t != p);
		break;
	case CPUCLOCK_VIRT:
		left = cputime_div_non_zero(cputime_sub(expires.cpu, val.cpu),
				       nthreads);
		do {
			if (likely(!(t->flags & PF_EXITING))) {
				ticks = cputime_add(virt_ticks(t), left);
				if (cputime_eq(t->it_virt_expires,
					       cputime_zero) ||
				    cputime_gt(t->it_virt_expires, ticks)) {
					t->it_virt_expires = ticks;
				}
			}
			t = next_thread(t);
		} while (t != p);
		break;
	case CPUCLOCK_SCHED:
		nsleft = expires.sched - val.sched;
		do_div(nsleft, nthreads);
		nsleft = max_t(unsigned long long, nsleft, 1);
		do {
			if (likely(!(t->flags & PF_EXITING))) {
				ns = t->se.sum_exec_runtime + nsleft;
				if (t->it_sched_expires == 0 ||
				    t->it_sched_expires > ns) {
					t->it_sched_expires = ns;
				}
			}
			t = next_thread(t);
		} while (t != p);
		break;
	}
}
