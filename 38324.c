static void __exit aac_exit(void)
{
	if (aac_cfg_major > -1)
		unregister_chrdev(aac_cfg_major, "aac");
	pci_unregister_driver(&aac_pci_driver);
}
