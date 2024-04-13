static void ip_mc_clear_src(struct ip_mc_list *pmc)
{
	struct ip_sf_list *psf, *nextpsf;

	for (psf=pmc->tomb; psf; psf=nextpsf) {
		nextpsf = psf->sf_next;
		kfree(psf);
	}
	pmc->tomb = NULL;
	for (psf=pmc->sources; psf; psf=nextpsf) {
		nextpsf = psf->sf_next;
		kfree(psf);
	}
	pmc->sources = NULL;
	pmc->sfmode = MCAST_EXCLUDE;
	pmc->sfcount[MCAST_INCLUDE] = 0;
	pmc->sfcount[MCAST_EXCLUDE] = 1;
}
