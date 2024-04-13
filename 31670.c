smp_fetch_hdr_names(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct hdr_idx *idx;
	struct hdr_ctx ctx;
	const struct http_msg *msg;
	struct chunk *temp;
	char del = ',';

	if (args && args->type == ARGT_STR)
		del = *args[0].data.str.str;

	CHECK_HTTP_MESSAGE_FIRST();

	idx = &smp->strm->txn->hdr_idx;
	msg = ((smp->opt & SMP_OPT_DIR) == SMP_OPT_DIR_REQ) ? &smp->strm->txn->req : &smp->strm->txn->rsp;

	temp = get_trash_chunk();

	ctx.idx = 0;
	while (http_find_next_header(msg->chn->buf->p, idx, &ctx)) {
		if (temp->len)
			temp->str[temp->len++] = del;
		memcpy(temp->str + temp->len, ctx.line, ctx.del);
		temp->len += ctx.del;
	}

	smp->data.type = SMP_T_STR;
	smp->data.u.str.str = temp->str;
	smp->data.u.str.len = temp->len;
	smp->flags = SMP_F_VOL_HDR;
	return 1;
}
