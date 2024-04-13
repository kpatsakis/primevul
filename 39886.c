static void __exit afiucv_exit(void)
{
	if (pr_iucv) {
		device_unregister(af_iucv_dev);
		driver_unregister(&af_iucv_driver);
		pr_iucv->iucv_unregister(&af_iucv_handler, 0);
		symbol_put(iucv_if);
	} else
		unregister_netdevice_notifier(&afiucv_netdev_notifier);
	dev_remove_pack(&iucv_packet_type);
	sock_unregister(PF_IUCV);
	proto_unregister(&iucv_proto);
}
