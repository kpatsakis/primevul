xfs_inherit_acl(struct inode *inode, struct posix_acl *acl)
{
	umode_t mode = inode->i_mode;
	int error = 0, inherit = 0;

	if (S_ISDIR(inode->i_mode)) {
		error = xfs_set_acl(inode, ACL_TYPE_DEFAULT, acl);
		if (error)
			goto out;
	}

	error = posix_acl_create(&acl, GFP_KERNEL, &mode);
	if (error < 0)
		return error;

	/*
	 * If posix_acl_create returns a positive value we need to
	 * inherit a permission that can't be represented using the Unix
	 * mode bits and we actually need to set an ACL.
	 */
	if (error > 0)
		inherit = 1;

	error = xfs_set_mode(inode, mode);
	if (error)
		goto out;

	if (inherit)
		error = xfs_set_acl(inode, ACL_TYPE_ACCESS, acl);

out:
	posix_acl_release(acl);
	return error;
}
