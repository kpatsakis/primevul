ftrace_graph_write(struct file *file, const char __user *ubuf,
		   size_t cnt, loff_t *ppos)
{
	struct trace_parser parser;
	ssize_t read, ret;

	if (!cnt)
		return 0;

	mutex_lock(&graph_lock);

	if (trace_parser_get_init(&parser, FTRACE_BUFF_MAX)) {
		ret = -ENOMEM;
		goto out_unlock;
	}

	read = trace_get_user(&parser, ubuf, cnt, ppos);

	if (read >= 0 && trace_parser_loaded((&parser))) {
		parser.buffer[parser.idx] = 0;

		/* we allow only one expression at a time */
		ret = ftrace_set_func(ftrace_graph_funcs, &ftrace_graph_count,
					parser.buffer);
		if (ret)
			goto out_free;
	}

	ret = read;

out_free:
	trace_parser_put(&parser);
out_unlock:
	mutex_unlock(&graph_lock);

	return ret;
}
