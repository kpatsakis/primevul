static void igmpv3_add_delrec(struct in_device *in_dev, struct ip_mc_list *im)
{
	struct ip_mc_list *pmc;

	/* this is an "ip_mc_list" for convenience; only the fields below
	 * are actually used. In particular, the refcnt and users are not
	 * used for management of the delete list. Using the same structure
	 * for deleted items allows change reports to use common code with
	 * non-deleted or query-response MCA's.
	 */
	pmc = kzalloc(sizeof(*pmc), GFP_KERNEL);
	if (!pmc)
		return;
	spin_lock_bh(&im->lock);
	pmc->interface = im->interface;
	in_dev_hold(in_dev);
	pmc->multiaddr = im->multiaddr;
	pmc->crcount = in_dev->mr_qrv ? in_dev->mr_qrv :
		IGMP_Unsolicited_Report_Count;
	pmc->sfmode = im->sfmode;
	if (pmc->sfmode == MCAST_INCLUDE) {
		struct ip_sf_list *psf;

		pmc->tomb = im->tomb;
		pmc->sources = im->sources;
		im->tomb = im->sources = NULL;
		for (psf=pmc->sources; psf; psf=psf->sf_next)
			psf->sf_crcount = pmc->crcount;
	}
	spin_unlock_bh(&im->lock);

	spin_lock_bh(&in_dev->mc_tomb_lock);
	pmc->next = in_dev->mc_tomb;
	in_dev->mc_tomb = pmc;
	spin_unlock_bh(&in_dev->mc_tomb_lock);
}
