static int __init aac_init(void)
{
	int error;

	printk(KERN_INFO "Adaptec %s driver %s\n",
	  AAC_DRIVERNAME, aac_driver_version);

	error = pci_register_driver(&aac_pci_driver);
	if (error < 0)
		return error;

	aac_cfg_major = register_chrdev( 0, "aac", &aac_cfg_fops);
	if (aac_cfg_major < 0) {
		printk(KERN_WARNING
			"aacraid: unable to register \"aac\" device.\n");
	}

	return 0;
}
