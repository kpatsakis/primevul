int ip_mc_leave_group(struct sock *sk, struct ip_mreqn *imr)
{
	struct inet_sock *inet = inet_sk(sk);
	struct ip_mc_socklist *iml;
	struct ip_mc_socklist __rcu **imlp;
	struct in_device *in_dev;
	struct net *net = sock_net(sk);
	__be32 group = imr->imr_multiaddr.s_addr;
	u32 ifindex;
	int ret = -EADDRNOTAVAIL;

	rtnl_lock();
	in_dev = ip_mc_find_dev(net, imr);
	ifindex = imr->imr_ifindex;
	for (imlp = &inet->mc_list;
	     (iml = rtnl_dereference(*imlp)) != NULL;
	     imlp = &iml->next_rcu) {
		if (iml->multi.imr_multiaddr.s_addr != group)
			continue;
		if (ifindex) {
			if (iml->multi.imr_ifindex != ifindex)
				continue;
		} else if (imr->imr_address.s_addr && imr->imr_address.s_addr !=
				iml->multi.imr_address.s_addr)
			continue;

		(void) ip_mc_leave_src(sk, iml, in_dev);

		*imlp = iml->next_rcu;

		if (in_dev)
			ip_mc_dec_group(in_dev, group);
		rtnl_unlock();
		/* decrease mem now to avoid the memleak warning */
		atomic_sub(sizeof(*iml), &sk->sk_omem_alloc);
		kfree_rcu(iml, rcu);
		return 0;
	}
	if (!in_dev)
		ret = -ENODEV;
	rtnl_unlock();
	return ret;
}
