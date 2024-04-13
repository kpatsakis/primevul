int vfs_slow_fallocate(files_struct *fsp, off_t offset, off_t len)
{
	ssize_t pwrite_ret;
	size_t total = 0;

	if (!sparse_buf) {
		sparse_buf = SMB_CALLOC_ARRAY(char, SPARSE_BUF_WRITE_SIZE);
		if (!sparse_buf) {
			errno = ENOMEM;
			return -1;
		}
	}

	while (total < len) {
		size_t curr_write_size = MIN(SPARSE_BUF_WRITE_SIZE, (len - total));

		pwrite_ret = SMB_VFS_PWRITE(fsp, sparse_buf, curr_write_size, offset + total);
		if (pwrite_ret == -1) {
			int saved_errno = errno;
			DEBUG(10,("vfs_slow_fallocate: SMB_VFS_PWRITE for file "
				  "%s failed with error %s\n",
				  fsp_str_dbg(fsp), strerror(saved_errno)));
			errno = saved_errno;
			return -1;
		}
		total += pwrite_ret;
	}

	return 0;
}
