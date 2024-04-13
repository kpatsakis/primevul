static int nfs_refresh_inode_locked(struct inode *inode, struct nfs_fattr *fattr)
{
	if (nfs_inode_attrs_need_update(inode, fattr))
		return nfs_update_inode(inode, fattr);
	return nfs_check_inode_attributes(inode, fattr);
}
