static void cleanup_timers(struct list_head *head,
			   cputime_t utime, cputime_t stime,
			   unsigned long long sum_exec_runtime)
{
	struct cpu_timer_list *timer, *next;
	cputime_t ptime = cputime_add(utime, stime);

	list_for_each_entry_safe(timer, next, head, entry) {
		list_del_init(&timer->entry);
		if (cputime_lt(timer->expires.cpu, ptime)) {
			timer->expires.cpu = cputime_zero;
		} else {
			timer->expires.cpu = cputime_sub(timer->expires.cpu,
							 ptime);
		}
	}

	++head;
	list_for_each_entry_safe(timer, next, head, entry) {
		list_del_init(&timer->entry);
		if (cputime_lt(timer->expires.cpu, utime)) {
			timer->expires.cpu = cputime_zero;
		} else {
			timer->expires.cpu = cputime_sub(timer->expires.cpu,
							 utime);
		}
	}

	++head;
	list_for_each_entry_safe(timer, next, head, entry) {
		list_del_init(&timer->entry);
		if (timer->expires.sched < sum_exec_runtime) {
			timer->expires.sched = 0;
		} else {
			timer->expires.sched -= sum_exec_runtime;
		}
	}
}
