static void __exit pmcraid_exit(void)
{
	pmcraid_netlink_release();
	unregister_chrdev_region(MKDEV(pmcraid_major, 0),
				 PMCRAID_MAX_ADAPTERS);
	pci_unregister_driver(&pmcraid_driver);
	class_destroy(pmcraid_class);
}
