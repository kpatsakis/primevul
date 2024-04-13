int http_replace_req_line(int action, const char *replace, int len,
                          struct proxy *px, struct stream *s)
{
	struct http_txn *txn = s->txn;
	char *cur_ptr, *cur_end;
	int offset = 0;
	int delta;

	switch (action) {
	case 0: // method
		cur_ptr = s->req.buf->p;
		cur_end = cur_ptr + txn->req.sl.rq.m_l;

		/* adjust req line offsets and lengths */
		delta = len - offset - (cur_end - cur_ptr);
		txn->req.sl.rq.m_l += delta;
		txn->req.sl.rq.u   += delta;
		txn->req.sl.rq.v   += delta;
		break;

	case 1: // path
		cur_ptr = http_get_path(txn);
		if (!cur_ptr)
			cur_ptr = s->req.buf->p + txn->req.sl.rq.u;

		cur_end = cur_ptr;
		while (cur_end < s->req.buf->p + txn->req.sl.rq.u + txn->req.sl.rq.u_l && *cur_end != '?')
			cur_end++;

		/* adjust req line offsets and lengths */
		delta = len - offset - (cur_end - cur_ptr);
		txn->req.sl.rq.u_l += delta;
		txn->req.sl.rq.v   += delta;
		break;

	case 2: // query
		offset = 1;
		cur_ptr = s->req.buf->p + txn->req.sl.rq.u;
		cur_end = cur_ptr + txn->req.sl.rq.u_l;
		while (cur_ptr < cur_end && *cur_ptr != '?')
			cur_ptr++;

		/* skip the question mark or indicate that we must insert it
		 * (but only if the format string is not empty then).
		 */
		if (cur_ptr < cur_end)
			cur_ptr++;
		else if (len > 1)
			offset = 0;

		/* adjust req line offsets and lengths */
		delta = len - offset - (cur_end - cur_ptr);
		txn->req.sl.rq.u_l += delta;
		txn->req.sl.rq.v   += delta;
		break;

	case 3: // uri
		cur_ptr = s->req.buf->p + txn->req.sl.rq.u;
		cur_end = cur_ptr + txn->req.sl.rq.u_l;

		/* adjust req line offsets and lengths */
		delta = len - offset - (cur_end - cur_ptr);
		txn->req.sl.rq.u_l += delta;
		txn->req.sl.rq.v   += delta;
		break;

	default:
		return -1;
	}

	/* commit changes and adjust end of message */
	delta = buffer_replace2(s->req.buf, cur_ptr, cur_end, replace + offset, len - offset);
	txn->req.sl.rq.l += delta;
	txn->hdr_idx.v[0].len += delta;
	http_msg_move_end(&txn->req, delta);
	return 0;
}
