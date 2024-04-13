int vfs_allocate_file_space(files_struct *fsp, uint64_t len)
{
	int ret;
	connection_struct *conn = fsp->conn;
	uint64_t space_avail;
	uint64_t bsize,dfree,dsize;
	NTSTATUS status;

	/*
	 * Actually try and commit the space on disk....
	 */

	DEBUG(10,("vfs_allocate_file_space: file %s, len %.0f\n",
		  fsp_str_dbg(fsp), (double)len));

	if (((off_t)len) < 0) {
		DEBUG(0,("vfs_allocate_file_space: %s negative len "
			 "requested.\n", fsp_str_dbg(fsp)));
		errno = EINVAL;
		return -1;
	}

	status = vfs_stat_fsp(fsp);
	if (!NT_STATUS_IS_OK(status)) {
		return -1;
	}

	if (len == (uint64_t)fsp->fsp_name->st.st_ex_size)
		return 0;

	if (len < (uint64_t)fsp->fsp_name->st.st_ex_size) {
		/* Shrink - use ftruncate. */

		DEBUG(10,("vfs_allocate_file_space: file %s, shrink. Current "
			  "size %.0f\n", fsp_str_dbg(fsp),
			  (double)fsp->fsp_name->st.st_ex_size));

		contend_level2_oplocks_begin(fsp, LEVEL2_CONTEND_ALLOC_SHRINK);

		flush_write_cache(fsp, SAMBA_SIZECHANGE_FLUSH);
		if ((ret = SMB_VFS_FTRUNCATE(fsp, (off_t)len)) != -1) {
			set_filelen_write_cache(fsp, len);
		}

		contend_level2_oplocks_end(fsp, LEVEL2_CONTEND_ALLOC_SHRINK);

		return ret;
	}

	/* Grow - we need to test if we have enough space. */

	contend_level2_oplocks_begin(fsp, LEVEL2_CONTEND_ALLOC_GROW);

	if (lp_strict_allocate(SNUM(fsp->conn))) {
		/* See if we have a syscall that will allocate beyond
		   end-of-file without changing EOF. */
		ret = SMB_VFS_FALLOCATE(fsp, VFS_FALLOCATE_FL_KEEP_SIZE,
					0, len);
	} else {
		ret = 0;
	}

	contend_level2_oplocks_end(fsp, LEVEL2_CONTEND_ALLOC_GROW);

	if (ret == 0) {
		/* We changed the allocation size on disk, but not
		   EOF - exactly as required. We're done ! */
		return 0;
	}

	if (ret == -1 && errno == ENOSPC) {
		return -1;
	}

	len -= fsp->fsp_name->st.st_ex_size;
	len /= 1024; /* Len is now number of 1k blocks needed. */
	space_avail = get_dfree_info(conn, fsp->fsp_name->base_name,
				     &bsize, &dfree, &dsize);
	if (space_avail == (uint64_t)-1) {
		return -1;
	}

	DEBUG(10,("vfs_allocate_file_space: file %s, grow. Current size %.0f, "
		  "needed blocks = %.0f, space avail = %.0f\n",
		  fsp_str_dbg(fsp), (double)fsp->fsp_name->st.st_ex_size, (double)len,
		  (double)space_avail));

	if (len > space_avail) {
		errno = ENOSPC;
		return -1;
	}

	return 0;
}
