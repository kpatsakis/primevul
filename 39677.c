static void t_stop(struct seq_file *m, void *p)
{
	int cpu;

	arch_spin_unlock(&max_stack_lock);

	cpu = smp_processor_id();
	per_cpu(trace_active, cpu)--;

	local_irq_enable();
}
