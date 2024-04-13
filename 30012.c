static ssize_t vfs_pread_fn(void *file, void *buf, size_t len, off_t offset)
{
	struct files_struct *fsp = (struct files_struct *)file;

	return SMB_VFS_PREAD(fsp, buf, len, offset);
}
