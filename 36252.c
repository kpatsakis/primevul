int ip_mc_gsfget(struct sock *sk, struct group_filter *gsf,
	struct group_filter __user *optval, int __user *optlen)
{
	int err, i, count, copycount;
	struct sockaddr_in *psin;
	__be32 addr;
	struct ip_mc_socklist *pmc;
	struct inet_sock *inet = inet_sk(sk);
	struct ip_sf_socklist *psl;

	psin = (struct sockaddr_in *)&gsf->gf_group;
	if (psin->sin_family != AF_INET)
		return -EINVAL;
	addr = psin->sin_addr.s_addr;
	if (!ipv4_is_multicast(addr))
		return -EINVAL;

	rtnl_lock();

	err = -EADDRNOTAVAIL;

	for_each_pmc_rtnl(inet, pmc) {
		if (pmc->multi.imr_multiaddr.s_addr == addr &&
		    pmc->multi.imr_ifindex == gsf->gf_interface)
			break;
	}
	if (!pmc)		/* must have a prior join */
		goto done;
	gsf->gf_fmode = pmc->sfmode;
	psl = rtnl_dereference(pmc->sflist);
	rtnl_unlock();
	count = psl ? psl->sl_count : 0;
	copycount = count < gsf->gf_numsrc ? count : gsf->gf_numsrc;
	gsf->gf_numsrc = count;
	if (put_user(GROUP_FILTER_SIZE(copycount), optlen) ||
	    copy_to_user(optval, gsf, GROUP_FILTER_SIZE(0))) {
		return -EFAULT;
	}
	for (i=0; i<copycount; i++) {
		struct sockaddr_storage ss;

		psin = (struct sockaddr_in *)&ss;
		memset(&ss, 0, sizeof(ss));
		psin->sin_family = AF_INET;
		psin->sin_addr.s_addr = psl->sl_addr[i];
		if (copy_to_user(&optval->gf_slist[i], &ss, sizeof(ss)))
			return -EFAULT;
	}
	return 0;
done:
	rtnl_unlock();
	return err;
}
