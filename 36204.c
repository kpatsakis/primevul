static void igmp_group_added(struct ip_mc_list *im)
{
	struct in_device *in_dev = im->interface;

	if (im->loaded == 0) {
		im->loaded = 1;
		ip_mc_filter_add(in_dev, im->multiaddr);
	}

#ifdef CONFIG_IP_MULTICAST
	if (im->multiaddr == IGMP_ALL_HOSTS)
		return;

	if (in_dev->dead)
		return;
	if (IGMP_V1_SEEN(in_dev) || IGMP_V2_SEEN(in_dev)) {
		spin_lock_bh(&im->lock);
		igmp_start_timer(im, IGMP_Initial_Report_Delay);
		spin_unlock_bh(&im->lock);
		return;
	}
	/* else, v3 */

	im->crcount = in_dev->mr_qrv ? in_dev->mr_qrv :
		IGMP_Unsolicited_Report_Count;
	igmp_ifc_event(in_dev);
#endif
}
