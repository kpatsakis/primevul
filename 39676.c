static void *t_start(struct seq_file *m, loff_t *pos)
{
	int cpu;

	local_irq_disable();

	cpu = smp_processor_id();
	per_cpu(trace_active, cpu)++;

	arch_spin_lock(&max_stack_lock);

	if (*pos == 0)
		return SEQ_START_TOKEN;

	return __next(m, pos);
}
