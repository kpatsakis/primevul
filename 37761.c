asmlinkage long sys_oabi_semop(int semid, struct oabi_sembuf __user *tsops,
			       unsigned nsops)
{
	return sys_oabi_semtimedop(semid, tsops, nsops, NULL);
}
