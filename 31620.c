void http_set_status(unsigned int status, const char *reason, struct stream *s)
{
	struct http_txn *txn = s->txn;
	char *cur_ptr, *cur_end;
	int delta;
	char *res;
	int c_l;
	const char *msg = reason;
	int msg_len;

	chunk_reset(&trash);

	res = ultoa_o(status, trash.str, trash.size);
	c_l = res - trash.str;

	trash.str[c_l] = ' ';
	trash.len = c_l + 1;

	/* Do we have a custom reason format string? */
	if (msg == NULL)
		msg = get_reason(status);
	msg_len = strlen(msg);
	strncpy(&trash.str[trash.len], msg, trash.size - trash.len);
	trash.len += msg_len;

	cur_ptr = s->res.buf->p + txn->rsp.sl.st.c;
	cur_end = s->res.buf->p + txn->rsp.sl.st.r + txn->rsp.sl.st.r_l;

	/* commit changes and adjust message */
	delta = buffer_replace2(s->res.buf, cur_ptr, cur_end, trash.str, trash.len);

	/* adjust res line offsets and lengths */
	txn->rsp.sl.st.r += c_l - txn->rsp.sl.st.c_l;
	txn->rsp.sl.st.c_l = c_l;
	txn->rsp.sl.st.r_l = msg_len;

	delta = trash.len - (cur_end - cur_ptr);
	txn->rsp.sl.st.l += delta;
	txn->hdr_idx.v[0].len += delta;
	http_msg_move_end(&txn->rsp, delta);
}
