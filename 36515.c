static int nfs_size_need_update(const struct inode *inode, const struct nfs_fattr *fattr)
{
	return nfs_size_to_loff_t(fattr->size) > i_size_read(inode);
}
