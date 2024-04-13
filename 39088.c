static int exitcode_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, exitcode_proc_show, NULL);
}
