bool kvm_vcpu_yield_to(struct kvm_vcpu *target)
{
	struct pid *pid;
	struct task_struct *task = NULL;
	bool ret = false;

	rcu_read_lock();
	pid = rcu_dereference(target->pid);
	if (pid)
		task = get_pid_task(target->pid, PIDTYPE_PID);
	rcu_read_unlock();
	if (!task)
		return ret;
	if (task->flags & PF_VCPU) {
		put_task_struct(task);
		return ret;
	}
	ret = yield_to(task, 1);
	put_task_struct(task);

	return ret;
}
