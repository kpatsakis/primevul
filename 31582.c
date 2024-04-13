void http_capture_bad_message(struct proxy *proxy, struct error_snapshot *es, struct stream *s,
                              struct http_msg *msg,
			      enum h1_state state, struct proxy *other_end)
{
	struct session *sess = strm_sess(s);
	struct channel *chn = msg->chn;
	int len1, len2;

	HA_SPIN_LOCK(PROXY_LOCK, &proxy->lock);
	es->len = MIN(chn->buf->i, global.tune.bufsize);
	len1 = chn->buf->data + chn->buf->size - chn->buf->p;
	len1 = MIN(len1, es->len);
	len2 = es->len - len1; /* remaining data if buffer wraps */

	if (!es->buf)
		es->buf = malloc(global.tune.bufsize);

	if (es->buf) {
		memcpy(es->buf, chn->buf->p, len1);
		if (len2)
			memcpy(es->buf + len1, chn->buf->data, len2);
	}

	if (msg->err_pos >= 0)
		es->pos = msg->err_pos;
	else
		es->pos = msg->next;

	es->when = date; // user-visible date
	es->sid  = s->uniq_id;
	es->srv  = objt_server(s->target);
	es->oe   = other_end;
	if (objt_conn(sess->origin))
		es->src  = __objt_conn(sess->origin)->addr.from;
	else
		memset(&es->src, 0, sizeof(es->src));

	es->state = state;
	es->ev_id = error_snapshot_id++;
	es->b_flags = chn->flags;
	es->s_flags = s->flags;
	es->t_flags = s->txn->flags;
	es->m_flags = msg->flags;
	es->b_out = chn->buf->o;
	es->b_wrap = chn->buf->data + chn->buf->size - chn->buf->p;
	es->b_tot = chn->total;
	es->m_clen = msg->chunk_len;
	es->m_blen = msg->body_len;
	HA_SPIN_UNLOCK(PROXY_LOCK, &proxy->lock);
}
