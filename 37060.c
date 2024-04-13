static int cpu_clock_sample_group(const clockid_t which_clock,
				  struct task_struct *p,
				  union cpu_time_count *cpu)
{
	int ret;
	unsigned long flags;
	spin_lock_irqsave(&p->sighand->siglock, flags);
	ret = cpu_clock_sample_group_locked(CPUCLOCK_WHICH(which_clock), p,
					    cpu);
	spin_unlock_irqrestore(&p->sighand->siglock, flags);
	return ret;
}
