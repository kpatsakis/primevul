cifs_d_revalidate(struct dentry *direntry, struct nameidata *nd)
{
	if (nd && (nd->flags & LOOKUP_RCU))
		return -ECHILD;

	if (direntry->d_inode) {
		if (cifs_revalidate_dentry(direntry))
			return 0;
		else {
			/*
			 * Forcibly invalidate automounting directory inodes
			 * (remote DFS directories) so to have them
			 * instantiated again for automount
			 */
			if (IS_AUTOMOUNT(direntry->d_inode))
				return 0;
			return 1;
		}
	}

	/*
	 * This may be nfsd (or something), anyway, we can't see the
	 * intent of this. So, since this can be for creation, drop it.
	 */
	if (!nd)
		return 0;

	/*
	 * Drop the negative dentry, in order to make sure to use the
	 * case sensitive name which is specified by user if this is
	 * for creation.
	 */
	if (nd->flags & (LOOKUP_CREATE | LOOKUP_RENAME_TARGET))
		return 0;

	if (time_after(jiffies, direntry->d_time + HZ) || !lookupCacheEnabled)
		return 0;

	return 1;
}
