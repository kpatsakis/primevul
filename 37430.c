static int __init pmcraid_init(void)
{
	dev_t dev;
	int error;

	pmcraid_info("%s Device Driver version: %s\n",
			 PMCRAID_DRIVER_NAME, PMCRAID_DRIVER_VERSION);

	error = alloc_chrdev_region(&dev, 0,
				    PMCRAID_MAX_ADAPTERS,
				    PMCRAID_DEVFILE);

	if (error) {
		pmcraid_err("failed to get a major number for adapters\n");
		goto out_init;
	}

	pmcraid_major = MAJOR(dev);
	pmcraid_class = class_create(THIS_MODULE, PMCRAID_DEVFILE);

	if (IS_ERR(pmcraid_class)) {
		error = PTR_ERR(pmcraid_class);
		pmcraid_err("failed to register with with sysfs, error = %x\n",
			    error);
		goto out_unreg_chrdev;
	}

	error = pmcraid_netlink_init();

	if (error)
		goto out_unreg_chrdev;

	error = pci_register_driver(&pmcraid_driver);

	if (error == 0)
		goto out_init;

	pmcraid_err("failed to register pmcraid driver, error = %x\n",
		     error);
	class_destroy(pmcraid_class);
	pmcraid_netlink_release();

out_unreg_chrdev:
	unregister_chrdev_region(MKDEV(pmcraid_major, 0), PMCRAID_MAX_ADAPTERS);

out_init:
	return error;
}
