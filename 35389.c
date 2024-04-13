static int ip6_frags_sysctl_register(void)
{
	ip6_ctl_header = register_net_sysctl_rotable(net_ipv6_ctl_path,
			ip6_frags_ctl_table);
	return ip6_ctl_header == NULL ? -ENOMEM : 0;
}
