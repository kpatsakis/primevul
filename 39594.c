ftrace_regex_write(struct file *file, const char __user *ubuf,
		   size_t cnt, loff_t *ppos, int enable)
{
	struct ftrace_iterator *iter;
	struct trace_parser *parser;
	ssize_t ret, read;

	if (!cnt)
		return 0;

	mutex_lock(&ftrace_regex_lock);

	ret = -ENODEV;
	if (unlikely(ftrace_disabled))
		goto out_unlock;

	if (file->f_mode & FMODE_READ) {
		struct seq_file *m = file->private_data;
		iter = m->private;
	} else
		iter = file->private_data;

	parser = &iter->parser;
	read = trace_get_user(parser, ubuf, cnt, ppos);

	if (read >= 0 && trace_parser_loaded(parser) &&
	    !trace_parser_cont(parser)) {
		ret = ftrace_process_regex(iter->hash, parser->buffer,
					   parser->idx, enable);
		trace_parser_clear(parser);
		if (ret)
			goto out_unlock;
	}

	ret = read;
out_unlock:
	mutex_unlock(&ftrace_regex_lock);

	return ret;
}
