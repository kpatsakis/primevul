static int nfs4_get_referral(struct inode *dir, const struct qstr *name, struct nfs_fattr *fattr, struct nfs_fh *fhandle)
{
	int status = -ENOMEM;
	struct page *page = NULL;
	struct nfs4_fs_locations *locations = NULL;

	page = alloc_page(GFP_KERNEL);
	if (page == NULL)
		goto out;
	locations = kmalloc(sizeof(struct nfs4_fs_locations), GFP_KERNEL);
	if (locations == NULL)
		goto out;

	status = nfs4_proc_fs_locations(dir, name, locations, page);
	if (status != 0)
		goto out;
	/* Make sure server returned a different fsid for the referral */
	if (nfs_fsid_equal(&NFS_SERVER(dir)->fsid, &locations->fattr.fsid)) {
		dprintk("%s: server did not return a different fsid for a referral at %s\n", __func__, name->name);
		status = -EIO;
		goto out;
	}

	memcpy(fattr, &locations->fattr, sizeof(struct nfs_fattr));
	fattr->valid |= NFS_ATTR_FATTR_V4_REFERRAL;
	if (!fattr->mode)
		fattr->mode = S_IFDIR;
	memset(fhandle, 0, sizeof(struct nfs_fh));
out:
	if (page)
		__free_page(page);
	if (locations)
		kfree(locations);
	return status;
}
