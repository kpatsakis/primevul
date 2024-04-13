static int __init afiucv_init(void)
{
	int err;

	if (MACHINE_IS_VM) {
		cpcmd("QUERY USERID", iucv_userid, sizeof(iucv_userid), &err);
		if (unlikely(err)) {
			WARN_ON(err);
			err = -EPROTONOSUPPORT;
			goto out;
		}

		pr_iucv = try_then_request_module(symbol_get(iucv_if), "iucv");
		if (!pr_iucv) {
			printk(KERN_WARNING "iucv_if lookup failed\n");
			memset(&iucv_userid, 0, sizeof(iucv_userid));
		}
	} else {
		memset(&iucv_userid, 0, sizeof(iucv_userid));
		pr_iucv = NULL;
	}

	err = proto_register(&iucv_proto, 0);
	if (err)
		goto out;
	err = sock_register(&iucv_sock_family_ops);
	if (err)
		goto out_proto;

	if (pr_iucv) {
		err = afiucv_iucv_init();
		if (err)
			goto out_sock;
	} else
		register_netdevice_notifier(&afiucv_netdev_notifier);
	dev_add_pack(&iucv_packet_type);
	return 0;

out_sock:
	sock_unregister(PF_IUCV);
out_proto:
	proto_unregister(&iucv_proto);
out:
	if (pr_iucv)
		symbol_put(iucv_if);
	return err;
}
