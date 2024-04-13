static inline cputime_t virt_ticks(struct task_struct *p)
{
	return p->utime;
}
