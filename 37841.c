int __init bnep_sock_init(void)
{
	int err;

	err = proto_register(&bnep_proto, 0);
	if (err < 0)
		return err;

	err = bt_sock_register(BTPROTO_BNEP, &bnep_sock_family_ops);
	if (err < 0)
		goto error;

	return 0;

error:
	BT_ERR("Can't register BNEP socket");
	proto_unregister(&bnep_proto);
	return err;
}
