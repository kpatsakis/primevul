static void uio_major_cleanup(void)
{
	unregister_chrdev_region(MKDEV(uio_major, 0), UIO_MAX_DEVICES);
	cdev_del(uio_cdev);
}
