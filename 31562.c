static int cli_parse_show_errors(char **args, struct appctx *appctx, void *private)
{
	if (!cli_has_level(appctx, ACCESS_LVL_OPER))
		return 1;

	if (*args[2]) {
		struct proxy *px;

		px = proxy_find_by_name(args[2], 0, 0);
		if (px)
			appctx->ctx.errors.iid = px->uuid;
		else
			appctx->ctx.errors.iid = atoi(args[2]);

		if (!appctx->ctx.errors.iid) {
			appctx->ctx.cli.severity = LOG_ERR;
			appctx->ctx.cli.msg = "No such proxy.\n";
			appctx->st0 = CLI_ST_PRINT;
			return 1;
		}
	}
	else
		appctx->ctx.errors.iid	= -1; // dump all proxies

	appctx->ctx.errors.flag = 0;
	if (strcmp(args[3], "request") == 0)
		appctx->ctx.errors.flag |= 4; // ignore response
	else if (strcmp(args[3], "response") == 0)
		appctx->ctx.errors.flag |= 2; // ignore request
	appctx->ctx.errors.px = NULL;
	return 0;
}
