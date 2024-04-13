asmlinkage long sys_oabi_fstat64(unsigned long fd,
				 struct oldabi_stat64 __user * statbuf)
{
	struct kstat stat;
	int error = vfs_fstat(fd, &stat);
	if (!error)
		error = cp_oldabi_stat64(&stat, statbuf);
	return error;
}
