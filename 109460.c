static int smsusb_init_device(struct usb_interface *intf, int board_id)
{
	struct smsdevice_params_t params;
	struct smsusb_device_t *dev;
	void *mdev;
	int i, rc;

	/* create device object */
	dev = kzalloc(sizeof(struct smsusb_device_t), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	memset(&params, 0, sizeof(params));
	usb_set_intfdata(intf, dev);
	dev->udev = interface_to_usbdev(intf);
	dev->state = SMSUSB_DISCONNECTED;

	params.device_type = sms_get_board(board_id)->type;

	switch (params.device_type) {
	case SMS_STELLAR:
		dev->buffer_size = USB1_BUFFER_SIZE;

		params.setmode_handler = smsusb1_setmode;
		params.detectmode_handler = smsusb1_detectmode;
		break;
	case SMS_UNKNOWN_TYPE:
		pr_err("Unspecified sms device type!\n");
		/* fall-thru */
	default:
		dev->buffer_size = USB2_BUFFER_SIZE;
		dev->response_alignment =
		    le16_to_cpu(dev->udev->ep_in[1]->desc.wMaxPacketSize) -
		    sizeof(struct sms_msg_hdr);

		params.flags |= SMS_DEVICE_FAMILY2;
		break;
	}

	for (i = 0; i < intf->cur_altsetting->desc.bNumEndpoints; i++) {
		if (intf->cur_altsetting->endpoint[i].desc. bEndpointAddress & USB_DIR_IN)
			dev->in_ep = intf->cur_altsetting->endpoint[i].desc.bEndpointAddress;
		else
			dev->out_ep = intf->cur_altsetting->endpoint[i].desc.bEndpointAddress;
	}

	pr_debug("in_ep = %02x, out_ep = %02x\n",
		dev->in_ep, dev->out_ep);

	params.device = &dev->udev->dev;
	params.usb_device = dev->udev;
	params.buffer_size = dev->buffer_size;
	params.num_buffers = MAX_BUFFERS;
	params.sendrequest_handler = smsusb_sendrequest;
	params.context = dev;
	usb_make_path(dev->udev, params.devpath, sizeof(params.devpath));

	mdev = siano_media_device_register(dev, board_id);

	/* register in smscore */
	rc = smscore_register_device(&params, &dev->coredev, 0, mdev);
	if (rc < 0) {
		pr_err("smscore_register_device(...) failed, rc %d\n", rc);
		smsusb_term_device(intf);
#ifdef CONFIG_MEDIA_CONTROLLER_DVB
		media_device_unregister(mdev);
#endif
		kfree(mdev);
		return rc;
	}

	smscore_set_board_id(dev->coredev, board_id);

	dev->coredev->is_usb_device = true;

	/* initialize urbs */
	for (i = 0; i < MAX_URBS; i++) {
		dev->surbs[i].dev = dev;
		usb_init_urb(&dev->surbs[i].urb);
	}

	pr_debug("smsusb_start_streaming(...).\n");
	rc = smsusb_start_streaming(dev);
	if (rc < 0) {
		pr_err("smsusb_start_streaming(...) failed\n");
		smsusb_term_device(intf);
		return rc;
	}

	dev->state = SMSUSB_ACTIVE;

	rc = smscore_start_device(dev->coredev);
	if (rc < 0) {
		pr_err("smscore_start_device(...) failed\n");
		smsusb_term_device(intf);
		return rc;
	}

	pr_debug("device 0x%p created\n", dev);

	return rc;
}