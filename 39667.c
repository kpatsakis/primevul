static __init int enable_stacktrace(char *str)
{
	if (strncmp(str, "_filter=", 8) == 0)
		strncpy(stack_trace_filter_buf, str+8, COMMAND_LINE_SIZE);

	stack_tracer_enabled = 1;
	last_stack_tracer_enabled = 1;
	return 1;
}
