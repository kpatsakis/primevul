static int uio_open(struct inode *inode, struct file *filep)
{
	struct uio_device *idev;
	struct uio_listener *listener;
	int ret = 0;

	mutex_lock(&minor_lock);
	idev = idr_find(&uio_idr, iminor(inode));
	mutex_unlock(&minor_lock);
	if (!idev) {
		ret = -ENODEV;
		goto out;
	}

	if (!try_module_get(idev->owner)) {
		ret = -ENODEV;
		goto out;
	}

	listener = kmalloc(sizeof(*listener), GFP_KERNEL);
	if (!listener) {
		ret = -ENOMEM;
		goto err_alloc_listener;
	}

	listener->dev = idev;
	listener->event_count = atomic_read(&idev->event);
	filep->private_data = listener;

	if (idev->info->open) {
		ret = idev->info->open(idev->info, inode);
		if (ret)
			goto err_infoopen;
	}
	return 0;

err_infoopen:
	kfree(listener);

err_alloc_listener:
	module_put(idev->owner);

out:
	return ret;
}
