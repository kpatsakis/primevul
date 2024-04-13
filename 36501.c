static int nfs_invalidate_mapping(struct inode *inode, struct address_space *mapping)
{
	int ret = 0;

	mutex_lock(&inode->i_mutex);
	if (NFS_I(inode)->cache_validity & NFS_INO_INVALID_DATA) {
		ret = nfs_sync_mapping(mapping);
		if (ret == 0)
			ret = nfs_invalidate_mapping_nolock(inode, mapping);
	}
	mutex_unlock(&inode->i_mutex);
	return ret;
}
