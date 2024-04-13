int cap_task_setscheduler(struct task_struct *p)
{
	return cap_safe_nice(p);
}
