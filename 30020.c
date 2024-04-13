off_t vfs_transfer_file(files_struct *in, files_struct *out, off_t n)
{
	return transfer_file_internal((void *)in, (void *)out, n,
				      vfs_pread_fn, vfs_pwrite_fn);
}
