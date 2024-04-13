static int init_uio_class(void)
{
	int ret;

	/* This is the first time in here, set everything up properly */
	ret = uio_major_init();
	if (ret)
		goto exit;

	ret = class_register(&uio_class);
	if (ret) {
		printk(KERN_ERR "class_register failed for uio\n");
		goto err_class_register;
	}
	return 0;

err_class_register:
	uio_major_cleanup();
exit:
	return ret;
}
