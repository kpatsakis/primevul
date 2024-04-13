smp_fetch_fhdr_cnt(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct hdr_idx *idx;
	struct hdr_ctx ctx;
	const struct http_msg *msg;
	int cnt;
	const char *name = NULL;
	int len = 0;

	if (args && args->type == ARGT_STR) {
		name = args->data.str.str;
		len = args->data.str.len;
	}

	CHECK_HTTP_MESSAGE_FIRST();

	idx = &smp->strm->txn->hdr_idx;
	msg = ((smp->opt & SMP_OPT_DIR) == SMP_OPT_DIR_REQ) ? &smp->strm->txn->req : &smp->strm->txn->rsp;

	ctx.idx = 0;
	cnt = 0;
	while (http_find_full_header2(name, len, msg->chn->buf->p, idx, &ctx))
		cnt++;

	smp->data.type = SMP_T_SINT;
	smp->data.u.sint = cnt;
	smp->flags = SMP_F_VOL_HDR;
	return 1;
}
