static int proc_fd_link(struct inode *inode, struct path *path)
{
	return proc_fd_info(inode, path, NULL);
}
