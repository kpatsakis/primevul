ftrace_regex_open(struct ftrace_ops *ops, int flag,
		  struct inode *inode, struct file *file)
{
	struct ftrace_iterator *iter;
	struct ftrace_hash *hash;
	int ret = 0;

	if (unlikely(ftrace_disabled))
		return -ENODEV;

	iter = kzalloc(sizeof(*iter), GFP_KERNEL);
	if (!iter)
		return -ENOMEM;

	if (trace_parser_get_init(&iter->parser, FTRACE_BUFF_MAX)) {
		kfree(iter);
		return -ENOMEM;
	}

	if (flag & FTRACE_ITER_NOTRACE)
		hash = ops->notrace_hash;
	else
		hash = ops->filter_hash;

	iter->ops = ops;
	iter->flags = flag;

	if (file->f_mode & FMODE_WRITE) {
		mutex_lock(&ftrace_lock);
		iter->hash = alloc_and_copy_ftrace_hash(FTRACE_HASH_DEFAULT_BITS, hash);
		mutex_unlock(&ftrace_lock);

		if (!iter->hash) {
			trace_parser_put(&iter->parser);
			kfree(iter);
			return -ENOMEM;
		}
	}

	mutex_lock(&ftrace_regex_lock);

	if ((file->f_mode & FMODE_WRITE) &&
	    (file->f_flags & O_TRUNC))
		ftrace_filter_reset(iter->hash);

	if (file->f_mode & FMODE_READ) {
		iter->pg = ftrace_pages_start;

		ret = seq_open(file, &show_ftrace_seq_ops);
		if (!ret) {
			struct seq_file *m = file->private_data;
			m->private = iter;
		} else {
			/* Failed */
			free_ftrace_hash(iter->hash);
			trace_parser_put(&iter->parser);
			kfree(iter);
		}
	} else
		file->private_data = iter;
	mutex_unlock(&ftrace_regex_lock);

	return ret;
}
