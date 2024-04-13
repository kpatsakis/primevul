smp_fetch_path(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct http_txn *txn;
	char *ptr, *end;

	CHECK_HTTP_MESSAGE_FIRST();

	txn = smp->strm->txn;
	end = txn->req.chn->buf->p + txn->req.sl.rq.u + txn->req.sl.rq.u_l;
	ptr = http_get_path(txn);
	if (!ptr)
		return 0;

	/* OK, we got the '/' ! */
	smp->data.type = SMP_T_STR;
	smp->data.u.str.str = ptr;

	while (ptr < end && *ptr != '?')
		ptr++;

	smp->data.u.str.len = ptr - smp->data.u.str.str;
	smp->flags = SMP_F_VOL_1ST | SMP_F_CONST;
	return 1;
}
