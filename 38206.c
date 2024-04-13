static int uio_release(struct inode *inode, struct file *filep)
{
	int ret = 0;
	struct uio_listener *listener = filep->private_data;
	struct uio_device *idev = listener->dev;

	if (idev->info->release)
		ret = idev->info->release(idev->info, inode);

	module_put(idev->owner);
	kfree(listener);
	return ret;
}
