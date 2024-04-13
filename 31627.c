static int http_upgrade_v09_to_v10(struct http_txn *txn)
{
	int delta;
	char *cur_end;
	struct http_msg *msg = &txn->req;

	if (msg->sl.rq.v_l != 0)
		return 1;

	/* RFC 1945 allows only GET for HTTP/0.9 requests */
	if (txn->meth != HTTP_METH_GET)
		return 0;

	cur_end = msg->chn->buf->p + msg->sl.rq.l;

	if (msg->sl.rq.u_l == 0) {
		/* HTTP/0.9 requests *must* have a request URI, per RFC 1945 */
		return 0;
	}
	/* add HTTP version */
	delta = buffer_replace2(msg->chn->buf, cur_end, cur_end, " HTTP/1.0\r\n", 11);
	http_msg_move_end(msg, delta);
	cur_end += delta;
	cur_end = (char *)http_parse_reqline(msg,
					     HTTP_MSG_RQMETH,
					     msg->chn->buf->p, cur_end + 1,
					     NULL, NULL);
	if (unlikely(!cur_end))
		return 0;

	/* we have a full HTTP/1.0 request now and we know that
	 * we have either a CR or an LF at <ptr>.
	 */
	hdr_idx_set_start(&txn->hdr_idx, msg->sl.rq.l, *cur_end == '\r');
	return 1;
}
