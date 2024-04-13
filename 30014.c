static ssize_t vfs_pwrite_fn(void *file, const void *buf, size_t len, off_t offset)
{
	struct files_struct *fsp = (struct files_struct *)file;

	return SMB_VFS_PWRITE(fsp, buf, len, offset);
}
