build_unc_path_to_root(const struct smb_vol *volume_info,
		const struct cifs_sb_info *cifs_sb)
{
	char *full_path;

	int unc_len = strnlen(volume_info->UNC, MAX_TREE_SIZE + 1);
	full_path = kmalloc(unc_len + cifs_sb->prepathlen + 1, GFP_KERNEL);
	if (full_path == NULL)
		return ERR_PTR(-ENOMEM);

	strncpy(full_path, volume_info->UNC, unc_len);
	if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_POSIX_PATHS) {
		int i;
		for (i = 0; i < unc_len; i++) {
			if (full_path[i] == '\\')
				full_path[i] = '/';
		}
	}

	if (cifs_sb->prepathlen)
		strncpy(full_path + unc_len, cifs_sb->prepath,
				cifs_sb->prepathlen);

	full_path[unc_len + cifs_sb->prepathlen] = 0; /* add trailing null */
	return full_path;
}
