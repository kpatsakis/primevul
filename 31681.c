smp_fetch_rqver(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct http_txn *txn;
	char *ptr;
	int len;

	CHECK_HTTP_MESSAGE_FIRST();

	txn = smp->strm->txn;
	len = txn->req.sl.rq.v_l;
	ptr = txn->req.chn->buf->p + txn->req.sl.rq.v;

	while ((len-- > 0) && (*ptr++ != '/'));
	if (len <= 0)
		return 0;

	smp->data.type = SMP_T_STR;
	smp->data.u.str.str = ptr;
	smp->data.u.str.len = len;

	smp->flags = SMP_F_VOL_1ST | SMP_F_CONST;
	return 1;
}
