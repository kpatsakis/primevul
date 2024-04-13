rio_init (void)
{
	return pci_register_driver(&rio_driver);
}
