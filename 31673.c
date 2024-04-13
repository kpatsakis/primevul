smp_fetch_http_auth(const struct arg *args, struct sample *smp, const char *kw, void *private)
{

	if (!args || args->type != ARGT_USR)
		return 0;

	CHECK_HTTP_MESSAGE_FIRST();

	if (!get_http_auth(smp->strm))
		return 0;

	smp->data.type = SMP_T_BOOL;
	smp->data.u.sint = check_user(args->data.usr, smp->strm->txn->auth.user,
	                            smp->strm->txn->auth.pass);
	return 1;
}
