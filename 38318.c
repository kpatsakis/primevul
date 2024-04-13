static long aac_cfg_ioctl(struct file *file,
		unsigned int cmd, unsigned long arg)
{
	int ret;
	if (!capable(CAP_SYS_RAWIO))
		return -EPERM;
	mutex_lock(&aac_mutex);
	ret = aac_do_ioctl(file->private_data, cmd, (void __user *)arg);
	mutex_unlock(&aac_mutex);

	return ret;
}
