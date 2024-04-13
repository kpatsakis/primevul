static void __exit l2tp_ip6_exit(void)
{
	inet6_unregister_protosw(&l2tp_ip6_protosw);
	inet6_del_protocol(&l2tp_ip6_protocol, IPPROTO_L2TP);
	proto_unregister(&l2tp_ip6_prot);
}
