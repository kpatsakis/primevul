xfs_xattr_acl_get(struct dentry *dentry, const char *name,
		void *value, size_t size, int type)
{
	struct posix_acl *acl;
	int error;

	acl = xfs_get_acl(dentry->d_inode, type);
	if (IS_ERR(acl))
		return PTR_ERR(acl);
	if (acl == NULL)
		return -ENODATA;

	error = posix_acl_to_xattr(acl, value, size);
	posix_acl_release(acl);

	return error;
}
