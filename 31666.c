smp_fetch_fhdr(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct hdr_idx *idx;
	struct hdr_ctx *ctx = smp->ctx.a[0];
	const struct http_msg *msg;
	int occ = 0;
	const char *name_str = NULL;
	int name_len = 0;

	if (!ctx) {
		/* first call */
		ctx = &static_hdr_ctx;
		ctx->idx = 0;
		smp->ctx.a[0] = ctx;
	}

	if (args) {
		if (args[0].type != ARGT_STR)
			return 0;
		name_str = args[0].data.str.str;
		name_len = args[0].data.str.len;

		if (args[1].type == ARGT_SINT)
			occ = args[1].data.sint;
	}

	CHECK_HTTP_MESSAGE_FIRST();

	idx = &smp->strm->txn->hdr_idx;
	msg = ((smp->opt & SMP_OPT_DIR) == SMP_OPT_DIR_REQ) ? &smp->strm->txn->req : &smp->strm->txn->rsp;

	if (ctx && !(smp->flags & SMP_F_NOT_LAST))
		/* search for header from the beginning */
		ctx->idx = 0;

	if (!occ && !(smp->opt & SMP_OPT_ITERATE))
		/* no explicit occurrence and single fetch => last header by default */
		occ = -1;

	if (!occ)
		/* prepare to report multiple occurrences for ACL fetches */
		smp->flags |= SMP_F_NOT_LAST;

	smp->data.type = SMP_T_STR;
	smp->flags |= SMP_F_VOL_HDR | SMP_F_CONST;
	if (http_get_fhdr(msg, name_str, name_len, idx, occ, ctx, &smp->data.u.str.str, &smp->data.u.str.len))
		return 1;

	smp->flags &= ~SMP_F_NOT_LAST;
	return 0;
}
