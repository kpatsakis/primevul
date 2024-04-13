smp_fetch_url(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct http_txn *txn;

	CHECK_HTTP_MESSAGE_FIRST();

	txn = smp->strm->txn;
	smp->data.type = SMP_T_STR;
	smp->data.u.str.len = txn->req.sl.rq.u_l;
	smp->data.u.str.str = txn->req.chn->buf->p + txn->req.sl.rq.u;
	smp->flags = SMP_F_VOL_1ST | SMP_F_CONST;
	return 1;
}
