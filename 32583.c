smp_fetch_cookie_val(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
                     const struct arg *args, struct sample *smp, const char *kw)
{
	int ret = smp_fetch_cookie(px, l4, l7, opt, args, smp, kw);

	if (ret > 0) {
		smp->type = SMP_T_UINT;
		smp->data.uint = strl2ic(smp->data.str.str, smp->data.str.len);
	}

	return ret;
}
