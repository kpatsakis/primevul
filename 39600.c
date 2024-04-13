ftrace_set_early_filter(struct ftrace_ops *ops, char *buf, int enable)
{
	char *func;

	while (buf) {
		func = strsep(&buf, ",");
		ftrace_set_regex(ops, func, strlen(func), 0, enable);
	}
}
