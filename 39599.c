ftrace_set_addr(struct ftrace_ops *ops, unsigned long ip, int remove,
		int reset, int enable)
{
	return ftrace_set_hash(ops, 0, 0, ip, remove, reset, enable);
}
