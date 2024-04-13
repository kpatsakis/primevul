static int mountstats_open(struct inode *inode, struct file *file)
{
	return mounts_open_common(inode, file, &mountstats_op);
}
