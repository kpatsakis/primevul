static int __init ip_vs_genl_register(void)
{
	return genl_register_family_with_ops(&ip_vs_genl_family,
		ip_vs_genl_ops, ARRAY_SIZE(ip_vs_genl_ops));
}
