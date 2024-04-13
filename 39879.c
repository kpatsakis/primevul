static int __init l2tp_ip6_init(void)
{
	int err;

	pr_info("L2TP IP encapsulation support for IPv6 (L2TPv3)\n");

	err = proto_register(&l2tp_ip6_prot, 1);
	if (err != 0)
		goto out;

	err = inet6_add_protocol(&l2tp_ip6_protocol, IPPROTO_L2TP);
	if (err)
		goto out1;

	inet6_register_protosw(&l2tp_ip6_protosw);
	return 0;

out1:
	proto_unregister(&l2tp_ip6_prot);
out:
	return err;
}
