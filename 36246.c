void ip_mc_destroy_dev(struct in_device *in_dev)
{
	struct ip_mc_list *i;

	ASSERT_RTNL();

	/* Deactivate timers */
	ip_mc_down(in_dev);

	while ((i = rtnl_dereference(in_dev->mc_list)) != NULL) {
		in_dev->mc_list = i->next_rcu;
		in_dev->mc_count--;

		/* We've dropped the groups in ip_mc_down already */
		ip_mc_clear_src(i);
		ip_ma_put(i);
	}
}
