smp_fetch_hdr_val(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	int ret = smp_fetch_hdr(args, smp, kw, private);

	if (ret > 0) {
		smp->data.type = SMP_T_SINT;
		smp->data.u.sint = strl2ic(smp->data.u.str.str, smp->data.u.str.len);
	}

	return ret;
}
