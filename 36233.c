static void igmpv3_del_delrec(struct in_device *in_dev, __be32 multiaddr)
{
	struct ip_mc_list *pmc, *pmc_prev;
	struct ip_sf_list *psf, *psf_next;

	spin_lock_bh(&in_dev->mc_tomb_lock);
	pmc_prev = NULL;
	for (pmc=in_dev->mc_tomb; pmc; pmc=pmc->next) {
		if (pmc->multiaddr == multiaddr)
			break;
		pmc_prev = pmc;
	}
	if (pmc) {
		if (pmc_prev)
			pmc_prev->next = pmc->next;
		else
			in_dev->mc_tomb = pmc->next;
	}
	spin_unlock_bh(&in_dev->mc_tomb_lock);
	if (pmc) {
		for (psf=pmc->tomb; psf; psf=psf_next) {
			psf_next = psf->sf_next;
			kfree(psf);
		}
		in_dev_put(pmc->interface);
		kfree(pmc);
	}
}
