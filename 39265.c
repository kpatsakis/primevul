cifs_setup_volume_info(struct smb_vol *volume_info, char *mount_data,
			const char *devname)
{
	int rc = 0;

	if (cifs_parse_mount_options(mount_data, devname, volume_info))
		return -EINVAL;

	if (volume_info->nullauth) {
		cifs_dbg(FYI, "Anonymous login\n");
		kfree(volume_info->username);
		volume_info->username = NULL;
	} else if (volume_info->username) {
		/* BB fixme parse for domain name here */
		cifs_dbg(FYI, "Username: %s\n", volume_info->username);
	} else {
		cifs_dbg(VFS, "No username specified\n");
	/* In userspace mount helper we can get user name from alternate
	   locations such as env variables and files on disk */
		return -EINVAL;
	}

	/* this is needed for ASCII cp to Unicode converts */
	if (volume_info->iocharset == NULL) {
		/* load_nls_default cannot return null */
		volume_info->local_nls = load_nls_default();
	} else {
		volume_info->local_nls = load_nls(volume_info->iocharset);
		if (volume_info->local_nls == NULL) {
			cifs_dbg(VFS, "CIFS mount error: iocharset %s not found\n",
				 volume_info->iocharset);
			return -ELIBACC;
		}
	}

	return rc;
}
