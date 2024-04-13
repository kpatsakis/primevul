stack_trace_call(unsigned long ip, unsigned long parent_ip,
		 struct ftrace_ops *op, struct pt_regs *pt_regs)
{
	int cpu;

	preempt_disable_notrace();

	cpu = raw_smp_processor_id();
	/* no atomic needed, we only modify this variable by this cpu */
	if (per_cpu(trace_active, cpu)++ != 0)
		goto out;

	check_stack();

 out:
	per_cpu(trace_active, cpu)--;
	/* prevent recursion in schedule */
	preempt_enable_notrace();
}
