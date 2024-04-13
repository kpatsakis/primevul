stack_trace_filter_open(struct inode *inode, struct file *file)
{
	return ftrace_regex_open(&trace_ops, FTRACE_ITER_FILTER,
				 inode, file);
}
