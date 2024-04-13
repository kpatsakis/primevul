smp_fetch_url_param(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct http_msg *msg;
	char delim = '?';
	const char *name;
	int name_len;

	if (!args ||
	    (args[0].type && args[0].type != ARGT_STR) ||
	    (args[1].type && args[1].type != ARGT_STR))
		return 0;

	name = "";
	name_len = 0;
	if (args->type == ARGT_STR) {
		name     = args->data.str.str;
		name_len = args->data.str.len;
	}

	if (args[1].type)
		delim = *args[1].data.str.str;

	if (!smp->ctx.a[0]) { // first call, find the query string
		CHECK_HTTP_MESSAGE_FIRST();

		msg = &smp->strm->txn->req;

		smp->ctx.a[0] = find_param_list(msg->chn->buf->p + msg->sl.rq.u,
		                                msg->sl.rq.u_l, delim);
		if (!smp->ctx.a[0])
			return 0;

		smp->ctx.a[1] = msg->chn->buf->p + msg->sl.rq.u + msg->sl.rq.u_l;

		/* Assume that the context is filled with NULL pointer
		 * before the first call.
		 * smp->ctx.a[2] = NULL;
		 * smp->ctx.a[3] = NULL;
		 */
	}

	return smp_fetch_param(delim, name, name_len, args, smp, kw, private);
}
