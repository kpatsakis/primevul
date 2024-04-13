static int proc_fd_permission(struct inode *inode, int mask, unsigned int flags)
{
	int rv = generic_permission(inode, mask, flags, NULL);
	if (rv == 0)
		return 0;
	if (task_pid(current) == proc_pid(inode))
		rv = 0;
	return rv;
}
