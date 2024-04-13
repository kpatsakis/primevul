int ip_mc_join_group(struct sock *sk , struct ip_mreqn *imr)
{
	int err;
	__be32 addr = imr->imr_multiaddr.s_addr;
	struct ip_mc_socklist *iml = NULL, *i;
	struct in_device *in_dev;
	struct inet_sock *inet = inet_sk(sk);
	struct net *net = sock_net(sk);
	int ifindex;
	int count = 0;

	if (!ipv4_is_multicast(addr))
		return -EINVAL;

	rtnl_lock();

	in_dev = ip_mc_find_dev(net, imr);

	if (!in_dev) {
		iml = NULL;
		err = -ENODEV;
		goto done;
	}

	err = -EADDRINUSE;
	ifindex = imr->imr_ifindex;
	for_each_pmc_rtnl(inet, i) {
		if (i->multi.imr_multiaddr.s_addr == addr &&
		    i->multi.imr_ifindex == ifindex)
			goto done;
		count++;
	}
	err = -ENOBUFS;
	if (count >= sysctl_igmp_max_memberships)
		goto done;
	iml = sock_kmalloc(sk, sizeof(*iml), GFP_KERNEL);
	if (iml == NULL)
		goto done;

	memcpy(&iml->multi, imr, sizeof(*imr));
	iml->next_rcu = inet->mc_list;
	iml->sflist = NULL;
	iml->sfmode = MCAST_EXCLUDE;
	RCU_INIT_POINTER(inet->mc_list, iml);
	ip_mc_inc_group(in_dev, addr);
	err = 0;
done:
	rtnl_unlock();
	return err;
}
