smp_fetch_meth(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	int meth;
	struct http_txn *txn;

	CHECK_HTTP_MESSAGE_FIRST_PERM();

	txn = smp->strm->txn;
	meth = txn->meth;
	smp->data.type = SMP_T_METH;
	smp->data.u.meth.meth = meth;
	if (meth == HTTP_METH_OTHER) {
		if (txn->rsp.msg_state != HTTP_MSG_RPBEFORE)
			/* ensure the indexes are not affected */
			return 0;
		smp->flags |= SMP_F_CONST;
		smp->data.u.meth.str.len = txn->req.sl.rq.m_l;
		smp->data.u.meth.str.str = txn->req.chn->buf->p;
	}
	smp->flags |= SMP_F_VOL_1ST;
	return 1;
}
