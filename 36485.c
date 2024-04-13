static int nfs_ctime_need_update(const struct inode *inode, const struct nfs_fattr *fattr)
{
	return timespec_compare(&fattr->ctime, &inode->i_ctime) > 0;
}
