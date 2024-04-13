smp_fetch_http_auth_grp(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
                        const struct arg *args, struct sample *smp, const char *kw)
{

	if (!args || args->type != ARGT_USR)
		return 0;

	CHECK_HTTP_MESSAGE_FIRST();

	if (!get_http_auth(l4))
		return 0;

	/* if the user does not belong to the userlist or has a wrong password,
	 * report that it unconditionally does not match. Otherwise we return
	 * a string containing the username.
	 */
	if (!check_user(args->data.usr, l4->txn.auth.user, l4->txn.auth.pass))
		return 0;

	/* pat_match_auth() will need the user list */
	smp->ctx.a[0] = args->data.usr;

	smp->type = SMP_T_STR;
	smp->flags = SMP_F_CONST;
	smp->data.str.str = l4->txn.auth.user;
	smp->data.str.len = strlen(l4->txn.auth.user);

	return 1;
}
