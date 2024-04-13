smp_fetch_uniqueid(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	if (LIST_ISEMPTY(&smp->sess->fe->format_unique_id))
		return 0;

	if (!smp->strm->unique_id) {
		if ((smp->strm->unique_id = pool_alloc(pool_head_uniqueid)) == NULL)
			return 0;
		smp->strm->unique_id[0] = '\0';
	}
	smp->data.u.str.len = build_logline(smp->strm, smp->strm->unique_id,
	                                    UNIQUEID_LEN, &smp->sess->fe->format_unique_id);

	smp->data.type = SMP_T_STR;
	smp->data.u.str.str = smp->strm->unique_id;
	smp->flags = SMP_F_CONST;
	return 1;
}
