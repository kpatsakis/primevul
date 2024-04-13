__next(struct seq_file *m, loff_t *pos)
{
	long n = *pos - 1;

	if (n >= max_stack_trace.nr_entries || stack_dump_trace[n] == ULONG_MAX)
		return NULL;

	m->private = (void *)n;
	return &m->private;
}
