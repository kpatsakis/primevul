static int __ip_vs_addr_is_local_v6(const struct in6_addr *addr)
{
	struct rt6_info *rt;
	struct flowi fl = {
		.oif = 0,
		.nl_u = {
			.ip6_u = {
				.daddr = *addr,
				.saddr = { .s6_addr32 = {0, 0, 0, 0} }, } },
	};

	rt = (struct rt6_info *)ip6_route_output(&init_net, NULL, &fl);
	if (rt && rt->rt6i_dev && (rt->rt6i_dev->flags & IFF_LOOPBACK))
			return 1;

	return 0;
}
