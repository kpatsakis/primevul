asmlinkage long sys_oabi_lstat64(const char __user * filename,
				 struct oldabi_stat64 __user * statbuf)
{
	struct kstat stat;
	int error = vfs_lstat(filename, &stat);
	if (!error)
		error = cp_oldabi_stat64(&stat, statbuf);
	return error;
}
