stack_trace_sysctl(struct ctl_table *table, int write,
		   void __user *buffer, size_t *lenp,
		   loff_t *ppos)
{
	int ret;

	mutex_lock(&stack_sysctl_mutex);

	ret = proc_dointvec(table, write, buffer, lenp, ppos);

	if (ret || !write ||
	    (last_stack_tracer_enabled == !!stack_tracer_enabled))
		goto out;

	last_stack_tracer_enabled = !!stack_tracer_enabled;

	if (stack_tracer_enabled)
		register_ftrace_function(&trace_ops);
	else
		unregister_ftrace_function(&trace_ops);

 out:
	mutex_unlock(&stack_sysctl_mutex);
	return ret;
}
