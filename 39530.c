ftrace_enabled_open(struct inode *inode, struct file *file)
{
	struct ftrace_iterator *iter;

	if (unlikely(ftrace_disabled))
		return -ENODEV;

	iter = __seq_open_private(file, &show_ftrace_seq_ops, sizeof(*iter));
	if (iter) {
		iter->pg = ftrace_pages_start;
		iter->flags = FTRACE_ITER_ENABLED;
		iter->ops = &global_ops;
	}

	return iter ? 0 : -ENOMEM;
}
