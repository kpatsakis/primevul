static __init int stack_trace_init(void)
{
	struct dentry *d_tracer;

	d_tracer = tracing_init_dentry();

	trace_create_file("stack_max_size", 0644, d_tracer,
			&max_stack_size, &stack_max_size_fops);

	trace_create_file("stack_trace", 0444, d_tracer,
			NULL, &stack_trace_fops);

	trace_create_file("stack_trace_filter", 0444, d_tracer,
			NULL, &stack_trace_filter_fops);

	if (stack_trace_filter_buf[0])
		ftrace_set_early_filter(&trace_ops, stack_trace_filter_buf, 1);

	if (stack_tracer_enabled)
		register_ftrace_function(&trace_ops);

	return 0;
}
