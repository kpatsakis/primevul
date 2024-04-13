static void *t_start(struct seq_file *m, loff_t *pos)
{
	struct ftrace_iterator *iter = m->private;
	struct ftrace_ops *ops = iter->ops;
	void *p = NULL;
	loff_t l;

	mutex_lock(&ftrace_lock);

	if (unlikely(ftrace_disabled))
		return NULL;

	/*
	 * If an lseek was done, then reset and start from beginning.
	 */
	if (*pos < iter->pos)
		reset_iter_read(iter);

	/*
	 * For set_ftrace_filter reading, if we have the filter
	 * off, we can short cut and just print out that all
	 * functions are enabled.
	 */
	if (iter->flags & FTRACE_ITER_FILTER &&
	    ftrace_hash_empty(ops->filter_hash)) {
		if (*pos > 0)
			return t_hash_start(m, pos);
		iter->flags |= FTRACE_ITER_PRINTALL;
		/* reset in case of seek/pread */
		iter->flags &= ~FTRACE_ITER_HASH;
		return iter;
	}

	if (iter->flags & FTRACE_ITER_HASH)
		return t_hash_start(m, pos);

	/*
	 * Unfortunately, we need to restart at ftrace_pages_start
	 * every time we let go of the ftrace_mutex. This is because
	 * those pointers can change without the lock.
	 */
	iter->pg = ftrace_pages_start;
	iter->idx = 0;
	for (l = 0; l <= *pos; ) {
		p = t_next(m, p, &l);
		if (!p)
			break;
	}

	if (!p)
		return t_hash_start(m, pos);

	return iter;
}
