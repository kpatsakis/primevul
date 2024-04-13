static int cap_safe_nice(struct task_struct *p)
{
	int is_subset;

	rcu_read_lock();
	is_subset = cap_issubset(__task_cred(p)->cap_permitted,
				 current_cred()->cap_permitted);
	rcu_read_unlock();

	if (!is_subset && !capable(CAP_SYS_NICE))
		return -EPERM;
	return 0;
}
