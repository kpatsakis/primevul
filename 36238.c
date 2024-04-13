int ip_check_mc_rcu(struct in_device *in_dev, __be32 mc_addr, __be32 src_addr, u16 proto)
{
	struct ip_mc_list *im;
	struct ip_sf_list *psf;
	int rv = 0;

	for_each_pmc_rcu(in_dev, im) {
		if (im->multiaddr == mc_addr)
			break;
	}
	if (im && proto == IPPROTO_IGMP) {
		rv = 1;
	} else if (im) {
		if (src_addr) {
			for (psf=im->sources; psf; psf=psf->sf_next) {
				if (psf->sf_inaddr == src_addr)
					break;
			}
			if (psf)
				rv = psf->sf_count[MCAST_INCLUDE] ||
					psf->sf_count[MCAST_EXCLUDE] !=
					im->sfcount[MCAST_EXCLUDE];
			else
				rv = im->sfcount[MCAST_EXCLUDE] != 0;
		} else
			rv = 1; /* unspecified source; tentatively allow */
	}
	return rv;
}
