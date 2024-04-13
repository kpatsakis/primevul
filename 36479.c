void nfs4_clear_inode(struct inode *inode)
{
	/* If we are holding a delegation, return it! */
	nfs_inode_return_delegation_noreclaim(inode);
	/* First call standard NFS clear_inode() code */
	nfs_clear_inode(inode);
}
