static ssize_t uio_write(struct file *filep, const char __user *buf,
			size_t count, loff_t *ppos)
{
	struct uio_listener *listener = filep->private_data;
	struct uio_device *idev = listener->dev;
	ssize_t retval;
	s32 irq_on;

	if (!idev->info->irq)
		return -EIO;

	if (count != sizeof(s32))
		return -EINVAL;

	if (!idev->info->irqcontrol)
		return -ENOSYS;

	if (copy_from_user(&irq_on, buf, count))
		return -EFAULT;

	retval = idev->info->irqcontrol(idev->info, irq_on);

	return retval ? retval : sizeof(s32);
}
