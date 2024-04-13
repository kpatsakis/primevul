int vfs_set_filelen(files_struct *fsp, off_t len)
{
	int ret;

	contend_level2_oplocks_begin(fsp, LEVEL2_CONTEND_SET_FILE_LEN);

	DEBUG(10,("vfs_set_filelen: ftruncate %s to len %.0f\n",
		  fsp_str_dbg(fsp), (double)len));
	flush_write_cache(fsp, SAMBA_SIZECHANGE_FLUSH);
	if ((ret = SMB_VFS_FTRUNCATE(fsp, len)) != -1) {
		set_filelen_write_cache(fsp, len);
		notify_fname(fsp->conn, NOTIFY_ACTION_MODIFIED,
			     FILE_NOTIFY_CHANGE_SIZE
			     | FILE_NOTIFY_CHANGE_ATTRIBUTES,
			     fsp->fsp_name->base_name);
	}

	contend_level2_oplocks_end(fsp, LEVEL2_CONTEND_SET_FILE_LEN);

	return ret;
}
