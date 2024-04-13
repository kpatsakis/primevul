char *vfs_GetWd(TALLOC_CTX *ctx, connection_struct *conn)
{
        char *current_dir = NULL;
	char *result = NULL;
	DATA_BLOB cache_value;
	struct file_id key;
	struct smb_filename *smb_fname_dot = NULL;
	struct smb_filename *smb_fname_full = NULL;

	if (!lp_getwd_cache()) {
		goto nocache;
	}

	smb_fname_dot = synthetic_smb_fname(ctx, ".", NULL, NULL);
	if (smb_fname_dot == NULL) {
		errno = ENOMEM;
		goto out;
	}

	if (SMB_VFS_STAT(conn, smb_fname_dot) == -1) {
		/*
		 * Known to fail for root: the directory may be NFS-mounted
		 * and exported with root_squash (so has no root access).
		 */
		DEBUG(1,("vfs_GetWd: couldn't stat \".\" error %s "
			 "(NFS problem ?)\n", strerror(errno) ));
		goto nocache;
	}

	key = vfs_file_id_from_sbuf(conn, &smb_fname_dot->st);

	if (!memcache_lookup(smbd_memcache(), GETWD_CACHE,
			     data_blob_const(&key, sizeof(key)),
			     &cache_value)) {
		goto nocache;
	}

	SMB_ASSERT((cache_value.length > 0)
		   && (cache_value.data[cache_value.length-1] == '\0'));

	smb_fname_full = synthetic_smb_fname(ctx, (char *)cache_value.data,
					     NULL, NULL);
	if (smb_fname_full == NULL) {
		errno = ENOMEM;
		goto out;
	}

	if ((SMB_VFS_STAT(conn, smb_fname_full) == 0) &&
	    (smb_fname_dot->st.st_ex_dev == smb_fname_full->st.st_ex_dev) &&
	    (smb_fname_dot->st.st_ex_ino == smb_fname_full->st.st_ex_ino) &&
	    (S_ISDIR(smb_fname_dot->st.st_ex_mode))) {
		/*
		 * Ok, we're done
		 */
		result = talloc_strdup(ctx, smb_fname_full->base_name);
		if (result == NULL) {
			errno = ENOMEM;
		}
		goto out;
	}

 nocache:

	/*
	 * We don't have the information to hand so rely on traditional
	 * methods. The very slow getcwd, which spawns a process on some
	 * systems, or the not quite so bad getwd.
	 */

	current_dir = SMB_VFS_GETWD(conn);
	if (current_dir == NULL) {
		DEBUG(0, ("vfs_GetWd: SMB_VFS_GETWD call failed: %s\n",
			  strerror(errno)));
		goto out;
	}

	if (lp_getwd_cache() && VALID_STAT(smb_fname_dot->st)) {
		key = vfs_file_id_from_sbuf(conn, &smb_fname_dot->st);

		memcache_add(smbd_memcache(), GETWD_CACHE,
			     data_blob_const(&key, sizeof(key)),
			     data_blob_const(current_dir,
						strlen(current_dir)+1));
	}

	result = talloc_strdup(ctx, current_dir);
	if (result == NULL) {
		errno = ENOMEM;
	}

 out:
	TALLOC_FREE(smb_fname_dot);
	TALLOC_FREE(smb_fname_full);
	SAFE_FREE(current_dir);
	return result;
}
