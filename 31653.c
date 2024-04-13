smp_fetch_body(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct http_msg *msg;
	unsigned long len;
	unsigned long block1;
	char *body;
	struct chunk *temp;

	CHECK_HTTP_MESSAGE_FIRST();

	if ((smp->opt & SMP_OPT_DIR) == SMP_OPT_DIR_REQ)
		msg = &smp->strm->txn->req;
	else
		msg = &smp->strm->txn->rsp;

	len  = http_body_bytes(msg);
	body = b_ptr(msg->chn->buf, -http_data_rewind(msg));

	block1 = len;
	if (block1 > msg->chn->buf->data + msg->chn->buf->size - body)
		block1 = msg->chn->buf->data + msg->chn->buf->size - body;

	if (block1 == len) {
		/* buffer is not wrapped (or empty) */
		smp->data.type = SMP_T_BIN;
		smp->data.u.str.str = body;
		smp->data.u.str.len = len;
		smp->flags = SMP_F_VOL_TEST | SMP_F_CONST;
	}
	else {
		/* buffer is wrapped, we need to defragment it */
		temp = get_trash_chunk();
		memcpy(temp->str, body, block1);
		memcpy(temp->str + block1, msg->chn->buf->data, len - block1);
		smp->data.type = SMP_T_BIN;
		smp->data.u.str.str = temp->str;
		smp->data.u.str.len = len;
		smp->flags = SMP_F_VOL_TEST;
	}
	return 1;
}
