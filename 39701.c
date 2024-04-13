static long vsock_dev_ioctl(struct file *filp,
			    unsigned int cmd, unsigned long arg)
{
	return vsock_dev_do_ioctl(filp, cmd, (void __user *)arg);
}
