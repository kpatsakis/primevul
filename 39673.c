static int stack_trace_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &stack_trace_seq_ops);
}
