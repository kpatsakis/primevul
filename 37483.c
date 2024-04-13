static int pmcraid_setup_chrdev(struct pmcraid_instance *pinstance)
{
	int minor;
	int error;

	minor = pmcraid_get_minor();
	cdev_init(&pinstance->cdev, &pmcraid_fops);
	pinstance->cdev.owner = THIS_MODULE;

	error = cdev_add(&pinstance->cdev, MKDEV(pmcraid_major, minor), 1);

	if (error)
		pmcraid_release_minor(minor);
	else
		device_create(pmcraid_class, NULL, MKDEV(pmcraid_major, minor),
			      NULL, "%s%u", PMCRAID_DEVFILE, minor);
	return error;
}
