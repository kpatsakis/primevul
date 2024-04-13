static long aac_compat_cfg_ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	if (!capable(CAP_SYS_RAWIO))
		return -EPERM;
	return aac_compat_do_ioctl(file->private_data, cmd, arg);
}
