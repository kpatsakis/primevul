static void igmp_heard_report(struct in_device *in_dev, __be32 group)
{
	struct ip_mc_list *im;

	/* Timers are only set for non-local groups */

	if (group == IGMP_ALL_HOSTS)
		return;

	rcu_read_lock();
	for_each_pmc_rcu(in_dev, im) {
		if (im->multiaddr == group) {
			igmp_stop_timer(im);
			break;
		}
	}
	rcu_read_unlock();
}
