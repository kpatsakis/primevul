int nfs_write_inode(struct inode *inode, int sync)
{
	int ret;

	if (sync) {
		ret = filemap_fdatawait(inode->i_mapping);
		if (ret == 0)
			ret = nfs_commit_inode(inode, FLUSH_SYNC);
	} else
		ret = nfs_commit_inode(inode, 0);
	if (ret >= 0)
		return 0;
	__mark_inode_dirty(inode, I_DIRTY_DATASYNC);
	return ret;
}
