smp_fetch_http_auth(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
                    const struct arg *args, struct sample *smp, const char *kw)
{

	if (!args || args->type != ARGT_USR)
		return 0;

	CHECK_HTTP_MESSAGE_FIRST();

	if (!get_http_auth(l4))
		return 0;

	smp->type = SMP_T_BOOL;
	smp->data.uint = check_user(args->data.usr, l4->txn.auth.user, l4->txn.auth.pass);
	return 1;
}
