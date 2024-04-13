static struct sk_buff *add_grec(struct sk_buff *skb, struct ip_mc_list *pmc,
	int type, int gdeleted, int sdeleted)
{
	struct net_device *dev = pmc->interface->dev;
	struct igmpv3_report *pih;
	struct igmpv3_grec *pgr = NULL;
	struct ip_sf_list *psf, *psf_next, *psf_prev, **psf_list;
	int scount, stotal, first, isquery, truncate;

	if (pmc->multiaddr == IGMP_ALL_HOSTS)
		return skb;

	isquery = type == IGMPV3_MODE_IS_INCLUDE ||
		  type == IGMPV3_MODE_IS_EXCLUDE;
	truncate = type == IGMPV3_MODE_IS_EXCLUDE ||
		    type == IGMPV3_CHANGE_TO_EXCLUDE;

	stotal = scount = 0;

	psf_list = sdeleted ? &pmc->tomb : &pmc->sources;

	if (!*psf_list)
		goto empty_source;

	pih = skb ? igmpv3_report_hdr(skb) : NULL;

	/* EX and TO_EX get a fresh packet, if needed */
	if (truncate) {
		if (pih && pih->ngrec &&
		    AVAILABLE(skb) < grec_size(pmc, type, gdeleted, sdeleted)) {
			if (skb)
				igmpv3_sendpack(skb);
			skb = igmpv3_newpack(dev, dev->mtu);
		}
	}
	first = 1;
	psf_prev = NULL;
	for (psf=*psf_list; psf; psf=psf_next) {
		__be32 *psrc;

		psf_next = psf->sf_next;

		if (!is_in(pmc, psf, type, gdeleted, sdeleted)) {
			psf_prev = psf;
			continue;
		}

		/* clear marks on query responses */
		if (isquery)
			psf->sf_gsresp = 0;

		if (AVAILABLE(skb) < sizeof(__be32) +
		    first*sizeof(struct igmpv3_grec)) {
			if (truncate && !first)
				break;	 /* truncate these */
			if (pgr)
				pgr->grec_nsrcs = htons(scount);
			if (skb)
				igmpv3_sendpack(skb);
			skb = igmpv3_newpack(dev, dev->mtu);
			first = 1;
			scount = 0;
		}
		if (first) {
			skb = add_grhead(skb, pmc, type, &pgr);
			first = 0;
		}
		if (!skb)
			return NULL;
		psrc = (__be32 *)skb_put(skb, sizeof(__be32));
		*psrc = psf->sf_inaddr;
		scount++; stotal++;
		if ((type == IGMPV3_ALLOW_NEW_SOURCES ||
		     type == IGMPV3_BLOCK_OLD_SOURCES) && psf->sf_crcount) {
			psf->sf_crcount--;
			if ((sdeleted || gdeleted) && psf->sf_crcount == 0) {
				if (psf_prev)
					psf_prev->sf_next = psf->sf_next;
				else
					*psf_list = psf->sf_next;
				kfree(psf);
				continue;
			}
		}
		psf_prev = psf;
	}

empty_source:
	if (!stotal) {
		if (type == IGMPV3_ALLOW_NEW_SOURCES ||
		    type == IGMPV3_BLOCK_OLD_SOURCES)
			return skb;
		if (pmc->crcount || isquery) {
			/* make sure we have room for group header */
			if (skb && AVAILABLE(skb)<sizeof(struct igmpv3_grec)) {
				igmpv3_sendpack(skb);
				skb = NULL; /* add_grhead will get a new one */
			}
			skb = add_grhead(skb, pmc, type, &pgr);
		}
	}
	if (pgr)
		pgr->grec_nsrcs = htons(scount);

	if (isquery)
		pmc->gsquery = 0;	/* clear query state on report */
	return skb;
}
