smp_fetch_base(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct http_txn *txn;
	char *ptr, *end, *beg;
	struct hdr_ctx ctx;
	struct chunk *temp;

	CHECK_HTTP_MESSAGE_FIRST();

	txn = smp->strm->txn;
	ctx.idx = 0;
	if (!http_find_header2("Host", 4, txn->req.chn->buf->p, &txn->hdr_idx, &ctx) || !ctx.vlen)
		return smp_fetch_path(args, smp, kw, private);

	/* OK we have the header value in ctx.line+ctx.val for ctx.vlen bytes */
	temp = get_trash_chunk();
	memcpy(temp->str, ctx.line + ctx.val, ctx.vlen);
	smp->data.type = SMP_T_STR;
	smp->data.u.str.str = temp->str;
	smp->data.u.str.len = ctx.vlen;

	/* now retrieve the path */
	end = txn->req.chn->buf->p + txn->req.sl.rq.u + txn->req.sl.rq.u_l;
	beg = http_get_path(txn);
	if (!beg)
		beg = end;

	for (ptr = beg; ptr < end && *ptr != '?'; ptr++);

	if (beg < ptr && *beg == '/') {
		memcpy(smp->data.u.str.str + smp->data.u.str.len, beg, ptr - beg);
		smp->data.u.str.len += ptr - beg;
	}

	smp->flags = SMP_F_VOL_1ST;
	return 1;
}
