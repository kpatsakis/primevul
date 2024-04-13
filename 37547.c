static int mounts_release(struct inode *inode, struct file *file)
{
	struct proc_mounts *p = file->private_data;
	path_put(&p->root);
	put_mnt_ns(p->ns);
	return seq_release(inode, file);
}
