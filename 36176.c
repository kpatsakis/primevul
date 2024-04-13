static int prctl_get_tid_address(struct task_struct *me, int __user **tid_addr)
{
	return put_user(me->clear_child_tid, tid_addr);
}
