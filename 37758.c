asmlinkage long sys_oabi_fstatat64(int dfd,
				   const char __user *filename,
				   struct oldabi_stat64  __user *statbuf,
				   int flag)
{
	struct kstat stat;
	int error;

	error = vfs_fstatat(dfd, filename, &stat, flag);
	if (error)
		return error;
	return cp_oldabi_stat64(&stat, statbuf);
}
