__g_next(struct seq_file *m, loff_t *pos)
{
	if (*pos >= ftrace_graph_count)
		return NULL;
	return &ftrace_graph_funcs[*pos];
}
