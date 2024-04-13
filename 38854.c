int __init ip_vs_control_init(void)
{
	int ret;
	int idx;

	EnterFunction(2);

	ret = nf_register_sockopt(&ip_vs_sockopts);
	if (ret) {
		pr_err("cannot register sockopt.\n");
		return ret;
	}

	ret = ip_vs_genl_register();
	if (ret) {
		pr_err("cannot register Generic Netlink interface.\n");
		nf_unregister_sockopt(&ip_vs_sockopts);
		return ret;
	}

	proc_net_fops_create(&init_net, "ip_vs", 0, &ip_vs_info_fops);
	proc_net_fops_create(&init_net, "ip_vs_stats",0, &ip_vs_stats_fops);

	sysctl_header = register_sysctl_paths(net_vs_ctl_path, vs_vars);

	/* Initialize ip_vs_svc_table, ip_vs_svc_fwm_table, ip_vs_rtable */
	for(idx = 0; idx < IP_VS_SVC_TAB_SIZE; idx++)  {
		INIT_LIST_HEAD(&ip_vs_svc_table[idx]);
		INIT_LIST_HEAD(&ip_vs_svc_fwm_table[idx]);
	}
	for(idx = 0; idx < IP_VS_RTAB_SIZE; idx++)  {
		INIT_LIST_HEAD(&ip_vs_rtable[idx]);
	}

	ip_vs_new_estimator(&ip_vs_stats);

	/* Hook the defense timer */
	schedule_delayed_work(&defense_work, DEFENSE_TIMER_PERIOD);

	LeaveFunction(2);
	return 0;
}
