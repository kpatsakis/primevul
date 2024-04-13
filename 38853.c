void ip_vs_control_cleanup(void)
{
	EnterFunction(2);
	ip_vs_trash_cleanup();
	cancel_rearming_delayed_work(&defense_work);
	cancel_work_sync(&defense_work.work);
	ip_vs_kill_estimator(&ip_vs_stats);
	unregister_sysctl_table(sysctl_header);
	proc_net_remove(&init_net, "ip_vs_stats");
	proc_net_remove(&init_net, "ip_vs");
	ip_vs_genl_unregister();
	nf_unregister_sockopt(&ip_vs_sockopts);
	LeaveFunction(2);
}
