static int mountinfo_open(struct inode *inode, struct file *file)
{
	return mounts_open_common(inode, file, &mountinfo_op);
}
