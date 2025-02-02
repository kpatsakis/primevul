int oz_cdev_deregister(void)
{
	cdev_del(&g_cdev.cdev);
	unregister_chrdev_region(g_cdev.devnum, 1);
	if (g_oz_class) {
		device_destroy(g_oz_class, g_cdev.devnum);
		class_destroy(g_oz_class);
	}
	return 0;
}
