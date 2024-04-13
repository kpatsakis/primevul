ftrace_filter_open(struct inode *inode, struct file *file)
{
	return ftrace_regex_open(&global_ops,
			FTRACE_ITER_FILTER | FTRACE_ITER_DO_HASH,
			inode, file);
}
