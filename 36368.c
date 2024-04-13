xfs_get_acl(struct inode *inode, int type)
{
	struct xfs_inode *ip = XFS_I(inode);
	struct posix_acl *acl;
	struct xfs_acl *xfs_acl;
	int len = sizeof(struct xfs_acl);
	unsigned char *ea_name;
	int error;

	acl = get_cached_acl(inode, type);
	if (acl != ACL_NOT_CACHED)
		return acl;

	trace_xfs_get_acl(ip);

	switch (type) {
	case ACL_TYPE_ACCESS:
		ea_name = SGI_ACL_FILE;
		break;
	case ACL_TYPE_DEFAULT:
		ea_name = SGI_ACL_DEFAULT;
		break;
	default:
		BUG();
	}

	/*
	 * If we have a cached ACLs value just return it, not need to
	 * go out to the disk.
	 */

	xfs_acl = kzalloc(sizeof(struct xfs_acl), GFP_KERNEL);
	if (!xfs_acl)
		return ERR_PTR(-ENOMEM);

	error = -xfs_attr_get(ip, ea_name, (unsigned char *)xfs_acl,
							&len, ATTR_ROOT);
	if (error) {
		/*
		 * If the attribute doesn't exist make sure we have a negative
		 * cache entry, for any other error assume it is transient and
		 * leave the cache entry as ACL_NOT_CACHED.
		 */
		if (error == -ENOATTR) {
			acl = NULL;
			goto out_update_cache;
		}
		goto out;
	}

	acl = xfs_acl_from_disk(xfs_acl);
	if (IS_ERR(acl))
		goto out;

 out_update_cache:
	set_cached_acl(inode, type, acl);
 out:
	kfree(xfs_acl);
	return acl;
}
