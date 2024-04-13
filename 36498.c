static int nfs_inode_attrs_need_update(const struct inode *inode, const struct nfs_fattr *fattr)
{
	const struct nfs_inode *nfsi = NFS_I(inode);

	return ((long)fattr->gencount - (long)nfsi->attr_gencount) > 0 ||
		nfs_ctime_need_update(inode, fattr) ||
		nfs_size_need_update(inode, fattr) ||
		((long)nfsi->attr_gencount - (long)nfs_read_attr_generation_counter() > 0);
}
