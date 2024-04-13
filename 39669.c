stack_max_size_write(struct file *filp, const char __user *ubuf,
		     size_t count, loff_t *ppos)
{
	long *ptr = filp->private_data;
	unsigned long val, flags;
	int ret;
	int cpu;

	ret = kstrtoul_from_user(ubuf, count, 10, &val);
	if (ret)
		return ret;

	local_irq_save(flags);

	/*
	 * In case we trace inside arch_spin_lock() or after (NMI),
	 * we will cause circular lock, so we also need to increase
	 * the percpu trace_active here.
	 */
	cpu = smp_processor_id();
	per_cpu(trace_active, cpu)++;

	arch_spin_lock(&max_stack_lock);
	*ptr = val;
	arch_spin_unlock(&max_stack_lock);

	per_cpu(trace_active, cpu)--;
	local_irq_restore(flags);

	return count;
}
