posix_acl_access_exists(struct inode *inode)
{
	return xfs_acl_exists(inode, SGI_ACL_FILE);
}
