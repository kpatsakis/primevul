static inline unsigned long long sched_ns(struct task_struct *p)
{
	return task_sched_runtime(p);
}
