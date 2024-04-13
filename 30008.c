int vfs_fill_sparse(files_struct *fsp, off_t len)
{
	int ret;
	NTSTATUS status;
	off_t offset;
	size_t num_to_write;

	status = vfs_stat_fsp(fsp);
	if (!NT_STATUS_IS_OK(status)) {
		return -1;
	}

	if (len <= fsp->fsp_name->st.st_ex_size) {
		return 0;
	}

#ifdef S_ISFIFO
	if (S_ISFIFO(fsp->fsp_name->st.st_ex_mode)) {
		return 0;
	}
#endif

	DEBUG(10,("vfs_fill_sparse: write zeros in file %s from len %.0f to "
		  "len %.0f (%.0f bytes)\n", fsp_str_dbg(fsp),
		  (double)fsp->fsp_name->st.st_ex_size, (double)len,
		  (double)(len - fsp->fsp_name->st.st_ex_size)));

	contend_level2_oplocks_begin(fsp, LEVEL2_CONTEND_FILL_SPARSE);

	flush_write_cache(fsp, SAMBA_SIZECHANGE_FLUSH);

	offset = fsp->fsp_name->st.st_ex_size;
	num_to_write = len - fsp->fsp_name->st.st_ex_size;

	/* Only do this on non-stream file handles. */
	if (fsp->base_fsp == NULL) {
		/* for allocation try fallocate first. This can fail on some
		 * platforms e.g. when the filesystem doesn't support it and no
		 * emulation is being done by the libc (like on AIX with JFS1). In that
		 * case we do our own emulation. fallocate implementations can
		 * return ENOTSUP or EINVAL in cases like that. */
		ret = SMB_VFS_FALLOCATE(fsp, 0, offset, num_to_write);
		if (ret == -1 && errno == ENOSPC) {
			goto out;
		}
		if (ret == 0) {
			goto out;
		}
		DEBUG(10,("vfs_fill_sparse: SMB_VFS_FALLOCATE failed with "
			"error %d. Falling back to slow manual allocation\n", ret));
	}

	ret = vfs_slow_fallocate(fsp, offset, num_to_write);

 out:

	if (ret == 0) {
		set_filelen_write_cache(fsp, len);
	}

	contend_level2_oplocks_end(fsp, LEVEL2_CONTEND_FILL_SPARSE);
	return ret;
}
