smp_fetch_meth(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
               const struct arg *args, struct sample *smp, const char *kw)
{
	int meth;
	struct http_txn *txn = l7;

	CHECK_HTTP_MESSAGE_FIRST_PERM();

	meth = txn->meth;
	smp->type = SMP_T_METH;
	smp->data.meth.meth = meth;
	if (meth == HTTP_METH_OTHER) {
		if (txn->rsp.msg_state != HTTP_MSG_RPBEFORE)
			/* ensure the indexes are not affected */
			return 0;
		smp->flags |= SMP_F_CONST;
		smp->data.meth.str.len = txn->req.sl.rq.m_l;
		smp->data.meth.str.str = txn->req.chn->buf->p;
	}
	smp->flags |= SMP_F_VOL_1ST;
	return 1;
}
