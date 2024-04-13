int oz_cdev_register(void)
{
	int err;
	struct device *dev;

	memset(&g_cdev, 0, sizeof(g_cdev));
	err = alloc_chrdev_region(&g_cdev.devnum, 0, 1, "ozwpan");
	if (err < 0)
		return err;
	oz_dbg(ON, "Alloc dev number %d:%d\n",
	       MAJOR(g_cdev.devnum), MINOR(g_cdev.devnum));
	cdev_init(&g_cdev.cdev, &oz_fops);
	g_cdev.cdev.owner = THIS_MODULE;
	g_cdev.cdev.ops = &oz_fops;
	spin_lock_init(&g_cdev.lock);
	init_waitqueue_head(&g_cdev.rdq);
	err = cdev_add(&g_cdev.cdev, g_cdev.devnum, 1);
	if (err < 0) {
		oz_dbg(ON, "Failed to add cdev\n");
		goto unregister;
	}
	g_oz_class = class_create(THIS_MODULE, "ozmo_wpan");
	if (IS_ERR(g_oz_class)) {
		oz_dbg(ON, "Failed to register ozmo_wpan class\n");
		err = PTR_ERR(g_oz_class);
		goto delete;
	}
	dev = device_create(g_oz_class, NULL, g_cdev.devnum, NULL, "ozwpan");
	if (IS_ERR(dev)) {
		oz_dbg(ON, "Failed to create sysfs entry for cdev\n");
		err = PTR_ERR(dev);
		goto delete;
	}
	return 0;

delete:
	cdev_del(&g_cdev.cdev);
unregister:
	unregister_chrdev_region(g_cdev.devnum, 1);
	return err;
}
