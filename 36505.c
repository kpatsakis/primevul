int nfs_post_op_update_inode_force_wcc(struct inode *inode, struct nfs_fattr *fattr)
{
	int status;

	spin_lock(&inode->i_lock);
	/* Don't do a WCC update if these attributes are already stale */
	if ((fattr->valid & NFS_ATTR_FATTR) == 0 ||
			!nfs_inode_attrs_need_update(inode, fattr)) {
		fattr->valid &= ~(NFS_ATTR_WCC_V4|NFS_ATTR_WCC);
		goto out_noforce;
	}
	if ((fattr->valid & NFS_ATTR_FATTR_V4) != 0 &&
			(fattr->valid & NFS_ATTR_WCC_V4) == 0) {
		fattr->pre_change_attr = NFS_I(inode)->change_attr;
		fattr->valid |= NFS_ATTR_WCC_V4;
	}
	if ((fattr->valid & NFS_ATTR_FATTR) != 0 &&
			(fattr->valid & NFS_ATTR_WCC) == 0) {
		memcpy(&fattr->pre_ctime, &inode->i_ctime, sizeof(fattr->pre_ctime));
		memcpy(&fattr->pre_mtime, &inode->i_mtime, sizeof(fattr->pre_mtime));
		fattr->pre_size = i_size_read(inode);
		fattr->valid |= NFS_ATTR_WCC;
	}
out_noforce:
	status = nfs_post_op_update_inode_locked(inode, fattr);
	spin_unlock(&inode->i_lock);
	return status;
}
