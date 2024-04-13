smp_fetch_stcode(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct http_txn *txn;
	char *ptr;
	int len;

	CHECK_HTTP_MESSAGE_FIRST();

	txn = smp->strm->txn;
	if (txn->rsp.msg_state < HTTP_MSG_BODY)
		return 0;

	len = txn->rsp.sl.st.c_l;
	ptr = txn->rsp.chn->buf->p + txn->rsp.sl.st.c;

	smp->data.type = SMP_T_SINT;
	smp->data.u.sint = __strl2ui(ptr, len);
	smp->flags = SMP_F_VOL_1ST;
	return 1;
}
