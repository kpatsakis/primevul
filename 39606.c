ftrace_set_regex(struct ftrace_ops *ops, unsigned char *buf, int len,
		 int reset, int enable)
{
	return ftrace_set_hash(ops, buf, len, 0, 0, reset, enable);
}
