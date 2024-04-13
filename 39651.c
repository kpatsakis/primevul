t_hash_show(struct seq_file *m, struct ftrace_iterator *iter)
{
	struct ftrace_func_probe *rec;

	rec = iter->probe;
	if (WARN_ON_ONCE(!rec))
		return -EIO;

	if (rec->ops->print)
		return rec->ops->print(m, rec->ip, rec->ops, rec->data);

	seq_printf(m, "%ps:%ps", (void *)rec->ip, (void *)rec->ops->func);

	if (rec->data)
		seq_printf(m, ":%p", rec->data);
	seq_putc(m, '\n');

	return 0;
}
