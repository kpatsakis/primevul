void __exit bnep_sock_cleanup(void)
{
	if (bt_sock_unregister(BTPROTO_BNEP) < 0)
		BT_ERR("Can't unregister BNEP socket");

	proto_unregister(&bnep_proto);
}
