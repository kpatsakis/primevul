smp_fetch_capture_header_req(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct proxy *fe = strm_fe(smp->strm);
	int idx;

	if (!args || args->type != ARGT_SINT)
		return 0;

	idx = args->data.sint;

	if (idx > (fe->nb_req_cap - 1) || smp->strm->req_cap == NULL || smp->strm->req_cap[idx] == NULL)
		return 0;

	smp->data.type = SMP_T_STR;
	smp->flags |= SMP_F_CONST;
	smp->data.u.str.str = smp->strm->req_cap[idx];
	smp->data.u.str.len = strlen(smp->strm->req_cap[idx]);

	return 1;
}
