smp_fetch_body_param(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct http_msg *msg;
	unsigned long len;
	unsigned long block1;
	char *body;
	const char *name;
	int name_len;

	if (!args || (args[0].type && args[0].type != ARGT_STR))
		return 0;

	name = "";
	name_len = 0;
	if (args[0].type == ARGT_STR) {
		name     = args[0].data.str.str;
		name_len = args[0].data.str.len;
	}

	if (!smp->ctx.a[0]) { // first call, find the query string
		CHECK_HTTP_MESSAGE_FIRST();

		if ((smp->opt & SMP_OPT_DIR) == SMP_OPT_DIR_REQ)
			msg = &smp->strm->txn->req;
		else
			msg = &smp->strm->txn->rsp;

		len  = http_body_bytes(msg);
		body = b_ptr(msg->chn->buf, -http_data_rewind(msg));

		block1 = len;
		if (block1 > msg->chn->buf->data + msg->chn->buf->size - body)
			block1 = msg->chn->buf->data + msg->chn->buf->size - body;

		if (block1 == len) {
			/* buffer is not wrapped (or empty) */
			smp->ctx.a[0] = body;
			smp->ctx.a[1] = body + len;

			/* Assume that the context is filled with NULL pointer
			 * before the first call.
			 * smp->ctx.a[2] = NULL;
			 * smp->ctx.a[3] = NULL;
			*/
		}
		else {
			/* buffer is wrapped, we need to defragment it */
			smp->ctx.a[0] = body;
			smp->ctx.a[1] = body + block1;
			smp->ctx.a[2] = msg->chn->buf->data;
			smp->ctx.a[3] = msg->chn->buf->data + ( len - block1 );
		}
	}
	return smp_fetch_param('&', name, name_len, args, smp, kw, private);
}
