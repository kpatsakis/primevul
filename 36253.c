void ip_mc_inc_group(struct in_device *in_dev, __be32 addr)
{
	struct ip_mc_list *im;

	ASSERT_RTNL();

	for_each_pmc_rtnl(in_dev, im) {
		if (im->multiaddr == addr) {
			im->users++;
			ip_mc_add_src(in_dev, &addr, MCAST_EXCLUDE, 0, NULL, 0);
			goto out;
		}
	}

	im = kzalloc(sizeof(*im), GFP_KERNEL);
	if (!im)
		goto out;

	im->users = 1;
	im->interface = in_dev;
	in_dev_hold(in_dev);
	im->multiaddr = addr;
	/* initial mode is (EX, empty) */
	im->sfmode = MCAST_EXCLUDE;
	im->sfcount[MCAST_EXCLUDE] = 1;
	atomic_set(&im->refcnt, 1);
	spin_lock_init(&im->lock);
#ifdef CONFIG_IP_MULTICAST
	setup_timer(&im->timer, &igmp_timer_expire, (unsigned long)im);
	im->unsolicit_count = IGMP_Unsolicited_Report_Count;
#endif

	im->next_rcu = in_dev->mc_list;
	in_dev->mc_count++;
	RCU_INIT_POINTER(in_dev->mc_list, im);

#ifdef CONFIG_IP_MULTICAST
	igmpv3_del_delrec(in_dev, im->multiaddr);
#endif
	igmp_group_added(im);
	if (!in_dev->dead)
		ip_rt_multicast_event(in_dev);
out:
	return;
}
