static int smp_conv_res_capture(const struct arg *args, struct sample *smp, void *private)
{
	struct proxy *fe = strm_fe(smp->strm);
	int idx, i;
	struct cap_hdr *hdr;
	int len;

	if (!args || args->type != ARGT_SINT)
		return 0;

	idx = args->data.sint;

	/* Check the availibity of the capture id. */
	if (idx > fe->nb_rsp_cap - 1)
		return 0;

	/* Look for the original configuration. */
	for (hdr = fe->rsp_cap, i = fe->nb_rsp_cap - 1;
	     hdr != NULL && i != idx ;
	     i--, hdr = hdr->next);
	if (!hdr)
		return 0;

	/* check for the memory allocation */
	if (smp->strm->res_cap[hdr->index] == NULL)
		smp->strm->res_cap[hdr->index] = pool_alloc(hdr->pool);
	if (smp->strm->res_cap[hdr->index] == NULL)
		return 0;

	/* Check length. */
	len = smp->data.u.str.len;
	if (len > hdr->len)
		len = hdr->len;

	/* Capture input data. */
	memcpy(smp->strm->res_cap[idx], smp->data.u.str.str, len);
	smp->strm->res_cap[idx][len] = '\0';

	return 1;
}
