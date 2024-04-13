static int uio_major_init(void)
{
	static const char name[] = "uio";
	struct cdev *cdev = NULL;
	dev_t uio_dev = 0;
	int result;

	result = alloc_chrdev_region(&uio_dev, 0, UIO_MAX_DEVICES, name);
	if (result)
		goto out;

	result = -ENOMEM;
	cdev = cdev_alloc();
	if (!cdev)
		goto out_unregister;

	cdev->owner = THIS_MODULE;
	cdev->ops = &uio_fops;
	kobject_set_name(&cdev->kobj, "%s", name);

	result = cdev_add(cdev, uio_dev, UIO_MAX_DEVICES);
	if (result)
		goto out_put;

	uio_major = MAJOR(uio_dev);
	uio_cdev = cdev;
	return 0;
out_put:
	kobject_put(&cdev->kobj);
out_unregister:
	unregister_chrdev_region(uio_dev, UIO_MAX_DEVICES);
out:
	return result;
}
