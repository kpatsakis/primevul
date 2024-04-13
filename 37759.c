asmlinkage int sys_oabi_ipc(uint call, int first, int second, int third,
			    void __user *ptr, long fifth)
{
	switch (call & 0xffff) {
	case SEMOP:
		return  sys_oabi_semtimedop(first,
					    (struct oabi_sembuf __user *)ptr,
					    second, NULL);
	case SEMTIMEDOP:
		return  sys_oabi_semtimedop(first,
					    (struct oabi_sembuf __user *)ptr,
					    second,
					    (const struct timespec __user *)fifth);
	default:
		return sys_ipc(call, first, second, third, ptr, fifth);
	}
}
