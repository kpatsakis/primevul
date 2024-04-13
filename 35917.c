int cap_task_setnice(struct task_struct *p, int nice)
{
	return cap_safe_nice(p);
}
