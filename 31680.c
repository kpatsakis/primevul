smp_fetch_query(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct http_txn *txn;
	char *ptr, *end;

	CHECK_HTTP_MESSAGE_FIRST();

	txn = smp->strm->txn;
	ptr = txn->req.chn->buf->p + txn->req.sl.rq.u;
	end = ptr + txn->req.sl.rq.u_l;

	/* look up the '?' */
	do {
		if (ptr == end)
			return 0;
	} while (*ptr++ != '?');

	smp->data.type = SMP_T_STR;
	smp->data.u.str.str = ptr;
	smp->data.u.str.len = end - ptr;
	smp->flags = SMP_F_VOL_1ST | SMP_F_CONST;
	return 1;
}
