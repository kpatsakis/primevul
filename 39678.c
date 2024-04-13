static int trace_lookup_stack(struct seq_file *m, long i)
{
	unsigned long addr = stack_dump_trace[i];

	return seq_printf(m, "%pS\n", (void *)addr);
}
