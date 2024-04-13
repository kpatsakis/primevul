xfs_acl_exists(struct inode *inode, unsigned char *name)
{
	int len = sizeof(struct xfs_acl);

	return (xfs_attr_get(XFS_I(inode), name, NULL, &len,
			    ATTR_ROOT|ATTR_KERNOVAL) == 0);
}
