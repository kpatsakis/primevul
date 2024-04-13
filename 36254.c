void ip_mc_init_dev(struct in_device *in_dev)
{
	ASSERT_RTNL();

	in_dev->mc_tomb = NULL;
#ifdef CONFIG_IP_MULTICAST
	in_dev->mr_gq_running = 0;
	setup_timer(&in_dev->mr_gq_timer, igmp_gq_timer_expire,
			(unsigned long)in_dev);
	in_dev->mr_ifc_count = 0;
	in_dev->mc_count     = 0;
	setup_timer(&in_dev->mr_ifc_timer, igmp_ifc_timer_expire,
			(unsigned long)in_dev);
	in_dev->mr_qrv = IGMP_Unsolicited_Report_Count;
#endif

	spin_lock_init(&in_dev->mc_tomb_lock);
}
