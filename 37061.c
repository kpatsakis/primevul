static int cpu_clock_sample_group_locked(unsigned int clock_idx,
					 struct task_struct *p,
					 union cpu_time_count *cpu)
{
	struct task_struct *t = p;
 	switch (clock_idx) {
	default:
		return -EINVAL;
	case CPUCLOCK_PROF:
		cpu->cpu = cputime_add(p->signal->utime, p->signal->stime);
		do {
			cpu->cpu = cputime_add(cpu->cpu, prof_ticks(t));
			t = next_thread(t);
		} while (t != p);
		break;
	case CPUCLOCK_VIRT:
		cpu->cpu = p->signal->utime;
		do {
			cpu->cpu = cputime_add(cpu->cpu, virt_ticks(t));
			t = next_thread(t);
		} while (t != p);
		break;
	case CPUCLOCK_SCHED:
		cpu->sched = p->signal->sum_sched_runtime;
		/* Add in each other live thread.  */
		while ((t = next_thread(t)) != p) {
			cpu->sched += t->se.sum_exec_runtime;
		}
		cpu->sched += sched_ns(p);
		break;
	}
	return 0;
}
