int __init igmp_mc_proc_init(void)
{
	return register_pernet_subsys(&igmp_net_ops);
}
