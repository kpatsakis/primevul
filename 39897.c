static int afiucv_iucv_init(void)
{
	int err;

	err = pr_iucv->iucv_register(&af_iucv_handler, 0);
	if (err)
		goto out;
	/* establish dummy device */
	af_iucv_driver.bus = pr_iucv->bus;
	err = driver_register(&af_iucv_driver);
	if (err)
		goto out_iucv;
	af_iucv_dev = kzalloc(sizeof(struct device), GFP_KERNEL);
	if (!af_iucv_dev) {
		err = -ENOMEM;
		goto out_driver;
	}
	dev_set_name(af_iucv_dev, "af_iucv");
	af_iucv_dev->bus = pr_iucv->bus;
	af_iucv_dev->parent = pr_iucv->root;
	af_iucv_dev->release = (void (*)(struct device *))kfree;
	af_iucv_dev->driver = &af_iucv_driver;
	err = device_register(af_iucv_dev);
	if (err)
		goto out_driver;
	return 0;

out_driver:
	driver_unregister(&af_iucv_driver);
out_iucv:
	pr_iucv->iucv_unregister(&af_iucv_handler, 0);
out:
	return err;
}
