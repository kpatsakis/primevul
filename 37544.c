static int mounts_open(struct inode *inode, struct file *file)
{
	return mounts_open_common(inode, file, &mounts_op);
}
