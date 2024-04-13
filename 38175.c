int __uio_register_device(struct module *owner,
			  struct device *parent,
			  struct uio_info *info)
{
	struct uio_device *idev;
	int ret = 0;

	if (!parent || !info || !info->name || !info->version)
		return -EINVAL;

	info->uio_dev = NULL;

	idev = kzalloc(sizeof(*idev), GFP_KERNEL);
	if (!idev) {
		ret = -ENOMEM;
		goto err_kzalloc;
	}

	idev->owner = owner;
	idev->info = info;
	init_waitqueue_head(&idev->wait);
	atomic_set(&idev->event, 0);

	ret = uio_get_minor(idev);
	if (ret)
		goto err_get_minor;

	idev->dev = device_create(&uio_class, parent,
				  MKDEV(uio_major, idev->minor), idev,
				  "uio%d", idev->minor);
	if (IS_ERR(idev->dev)) {
		printk(KERN_ERR "UIO: device register failed\n");
		ret = PTR_ERR(idev->dev);
		goto err_device_create;
	}

	ret = uio_dev_add_attributes(idev);
	if (ret)
		goto err_uio_dev_add_attributes;

	info->uio_dev = idev;

	if (info->irq && (info->irq != UIO_IRQ_CUSTOM)) {
		ret = request_irq(info->irq, uio_interrupt,
				  info->irq_flags, info->name, idev);
		if (ret)
			goto err_request_irq;
	}

	return 0;

err_request_irq:
	uio_dev_del_attributes(idev);
err_uio_dev_add_attributes:
	device_destroy(&uio_class, MKDEV(uio_major, idev->minor));
err_device_create:
	uio_free_minor(idev);
err_get_minor:
	kfree(idev);
err_kzalloc:
	return ret;
}
