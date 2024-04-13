void ip_mc_dec_group(struct in_device *in_dev, __be32 addr)
{
	struct ip_mc_list *i;
	struct ip_mc_list __rcu **ip;

	ASSERT_RTNL();

	for (ip = &in_dev->mc_list;
	     (i = rtnl_dereference(*ip)) != NULL;
	     ip = &i->next_rcu) {
		if (i->multiaddr == addr) {
			if (--i->users == 0) {
				*ip = i->next_rcu;
				in_dev->mc_count--;
				igmp_group_dropped(i);
				ip_mc_clear_src(i);

				if (!in_dev->dead)
					ip_rt_multicast_event(in_dev);

				ip_ma_put(i);
				return;
			}
			break;
		}
	}
}
