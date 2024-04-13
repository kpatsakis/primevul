static int nfs_invalidate_mapping_nolock(struct inode *inode, struct address_space *mapping)
{
	struct nfs_inode *nfsi = NFS_I(inode);
	
	if (mapping->nrpages != 0) {
		int ret = invalidate_inode_pages2(mapping);
		if (ret < 0)
			return ret;
	}
	spin_lock(&inode->i_lock);
	nfsi->cache_validity &= ~NFS_INO_INVALID_DATA;
	if (S_ISDIR(inode->i_mode))
		memset(nfsi->cookieverf, 0, sizeof(nfsi->cookieverf));
	spin_unlock(&inode->i_lock);
	nfs_inc_stats(inode, NFSIOS_DATAINVALIDATE);
	dfprintk(PAGECACHE, "NFS: (%s/%Ld) data cache invalidated\n",
			inode->i_sb->s_id, (long long)NFS_FILEID(inode));
	return 0;
}
