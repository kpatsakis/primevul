ftrace_graph_open(struct inode *inode, struct file *file)
{
	int ret = 0;

	if (unlikely(ftrace_disabled))
		return -ENODEV;

	mutex_lock(&graph_lock);
	if ((file->f_mode & FMODE_WRITE) &&
	    (file->f_flags & O_TRUNC)) {
		ftrace_graph_filter_enabled = 0;
		ftrace_graph_count = 0;
		memset(ftrace_graph_funcs, 0, sizeof(ftrace_graph_funcs));
	}
	mutex_unlock(&graph_lock);

	if (file->f_mode & FMODE_READ)
		ret = seq_open(file, &ftrace_graph_seq_ops);

	return ret;
}
