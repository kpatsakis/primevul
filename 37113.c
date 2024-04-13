asmlinkage long sys_time(time_t __user * tloc)
{
	time_t i = get_seconds();

	if (tloc) {
		if (put_user(i,tloc))
			i = -EFAULT;
	}
	return i;
}
