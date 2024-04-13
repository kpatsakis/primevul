expand_dfs_referral(const unsigned int xid, struct cifs_ses *ses,
		    struct smb_vol *volume_info, struct cifs_sb_info *cifs_sb,
		    int check_prefix)
{
	int rc;
	unsigned int num_referrals = 0;
	struct dfs_info3_param *referrals = NULL;
	char *full_path = NULL, *ref_path = NULL, *mdata = NULL;

	full_path = build_unc_path_to_root(volume_info, cifs_sb);
	if (IS_ERR(full_path))
		return PTR_ERR(full_path);

	/* For DFS paths, skip the first '\' of the UNC */
	ref_path = check_prefix ? full_path + 1 : volume_info->UNC + 1;

	rc = get_dfs_path(xid, ses, ref_path, cifs_sb->local_nls,
			  &num_referrals, &referrals,
			  cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);

	if (!rc && num_referrals > 0) {
		char *fake_devname = NULL;

		mdata = cifs_compose_mount_options(cifs_sb->mountdata,
						   full_path + 1, referrals,
						   &fake_devname);

		free_dfs_info_array(referrals, num_referrals);

		if (IS_ERR(mdata)) {
			rc = PTR_ERR(mdata);
			mdata = NULL;
		} else {
			cleanup_volume_info_contents(volume_info);
			rc = cifs_setup_volume_info(volume_info, mdata,
							fake_devname);
		}
		kfree(fake_devname);
		kfree(cifs_sb->mountdata);
		cifs_sb->mountdata = mdata;
	}
	kfree(full_path);
	return rc;
}
