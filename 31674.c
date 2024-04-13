smp_fetch_http_auth_grp(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	if (!args || args->type != ARGT_USR)
		return 0;

	CHECK_HTTP_MESSAGE_FIRST();

	if (!get_http_auth(smp->strm))
		return 0;

	/* if the user does not belong to the userlist or has a wrong password,
	 * report that it unconditionally does not match. Otherwise we return
	 * a string containing the username.
	 */
	if (!check_user(args->data.usr, smp->strm->txn->auth.user,
	                smp->strm->txn->auth.pass))
		return 0;

	/* pat_match_auth() will need the user list */
	smp->ctx.a[0] = args->data.usr;

	smp->data.type = SMP_T_STR;
	smp->flags = SMP_F_CONST;
	smp->data.u.str.str = smp->strm->txn->auth.user;
	smp->data.u.str.len = strlen(smp->strm->txn->auth.user);

	return 1;
}
