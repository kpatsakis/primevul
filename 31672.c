smp_fetch_hdrs(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct http_msg *msg;
	struct hdr_idx *idx;
	struct http_txn *txn;

	CHECK_HTTP_MESSAGE_FIRST();

	txn = smp->strm->txn;
	idx = &txn->hdr_idx;
	msg = &txn->req;

	smp->data.type = SMP_T_STR;
	smp->data.u.str.str = msg->chn->buf->p + hdr_idx_first_pos(idx);
	smp->data.u.str.len = msg->eoh - hdr_idx_first_pos(idx) + 1 +
	                      (msg->chn->buf->p[msg->eoh] == '\r');

	return 1;
}
