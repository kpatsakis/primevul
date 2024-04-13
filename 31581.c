static int http_apply_redirect_rule(struct redirect_rule *rule, struct stream *s, struct http_txn *txn)
{
	struct http_msg *req = &txn->req;
	struct http_msg *res = &txn->rsp;
	const char *msg_fmt;
	struct chunk *chunk;
	int ret = 0;

	chunk = alloc_trash_chunk();
	if (!chunk)
		goto leave;

	/* build redirect message */
	switch(rule->code) {
	case 308:
		msg_fmt = HTTP_308;
		break;
	case 307:
		msg_fmt = HTTP_307;
		break;
	case 303:
		msg_fmt = HTTP_303;
		break;
	case 301:
		msg_fmt = HTTP_301;
		break;
	case 302:
	default:
		msg_fmt = HTTP_302;
		break;
	}

	if (unlikely(!chunk_strcpy(chunk, msg_fmt)))
		goto leave;

	switch(rule->type) {
	case REDIRECT_TYPE_SCHEME: {
		const char *path;
		const char *host;
		struct hdr_ctx ctx;
		int pathlen;
		int hostlen;

		host = "";
		hostlen = 0;
		ctx.idx = 0;
		if (http_find_header2("Host", 4, req->chn->buf->p, &txn->hdr_idx, &ctx)) {
			host = ctx.line + ctx.val;
			hostlen = ctx.vlen;
		}

		path = http_get_path(txn);
		/* build message using path */
		if (path) {
			pathlen = req->sl.rq.u_l + (req->chn->buf->p + req->sl.rq.u) - path;
			if (rule->flags & REDIRECT_FLAG_DROP_QS) {
				int qs = 0;
				while (qs < pathlen) {
					if (path[qs] == '?') {
						pathlen = qs;
						break;
					}
					qs++;
				}
			}
		} else {
			path = "/";
			pathlen = 1;
		}

		if (rule->rdr_str) { /* this is an old "redirect" rule */
			/* check if we can add scheme + "://" + host + path */
			if (chunk->len + rule->rdr_len + 3 + hostlen + pathlen > chunk->size - 4)
				goto leave;

			/* add scheme */
			memcpy(chunk->str + chunk->len, rule->rdr_str, rule->rdr_len);
			chunk->len += rule->rdr_len;
		}
		else {
			/* add scheme with executing log format */
			chunk->len += build_logline(s, chunk->str + chunk->len, chunk->size - chunk->len, &rule->rdr_fmt);

			/* check if we can add scheme + "://" + host + path */
			if (chunk->len + 3 + hostlen + pathlen > chunk->size - 4)
				goto leave;
		}
		/* add "://" */
		memcpy(chunk->str + chunk->len, "://", 3);
		chunk->len += 3;

		/* add host */
		memcpy(chunk->str + chunk->len, host, hostlen);
		chunk->len += hostlen;

		/* add path */
		memcpy(chunk->str + chunk->len, path, pathlen);
		chunk->len += pathlen;

		/* append a slash at the end of the location if needed and missing */
		if (chunk->len && chunk->str[chunk->len - 1] != '/' &&
		    (rule->flags & REDIRECT_FLAG_APPEND_SLASH)) {
			if (chunk->len > chunk->size - 5)
				goto leave;
			chunk->str[chunk->len] = '/';
			chunk->len++;
		}

		break;
	}
	case REDIRECT_TYPE_PREFIX: {
		const char *path;
		int pathlen;

		path = http_get_path(txn);
		/* build message using path */
		if (path) {
			pathlen = req->sl.rq.u_l + (req->chn->buf->p + req->sl.rq.u) - path;
			if (rule->flags & REDIRECT_FLAG_DROP_QS) {
				int qs = 0;
				while (qs < pathlen) {
					if (path[qs] == '?') {
						pathlen = qs;
						break;
					}
					qs++;
				}
			}
		} else {
			path = "/";
			pathlen = 1;
		}

		if (rule->rdr_str) { /* this is an old "redirect" rule */
			if (chunk->len + rule->rdr_len + pathlen > chunk->size - 4)
				goto leave;

			/* add prefix. Note that if prefix == "/", we don't want to
			 * add anything, otherwise it makes it hard for the user to
			 * configure a self-redirection.
			 */
			if (rule->rdr_len != 1 || *rule->rdr_str != '/') {
				memcpy(chunk->str + chunk->len, rule->rdr_str, rule->rdr_len);
				chunk->len += rule->rdr_len;
			}
		}
		else {
			/* add prefix with executing log format */
			chunk->len += build_logline(s, chunk->str + chunk->len, chunk->size - chunk->len, &rule->rdr_fmt);

			/* Check length */
			if (chunk->len + pathlen > chunk->size - 4)
				goto leave;
		}

		/* add path */
		memcpy(chunk->str + chunk->len, path, pathlen);
		chunk->len += pathlen;

		/* append a slash at the end of the location if needed and missing */
		if (chunk->len && chunk->str[chunk->len - 1] != '/' &&
		    (rule->flags & REDIRECT_FLAG_APPEND_SLASH)) {
			if (chunk->len > chunk->size - 5)
				goto leave;
			chunk->str[chunk->len] = '/';
			chunk->len++;
		}

		break;
	}
	case REDIRECT_TYPE_LOCATION:
	default:
		if (rule->rdr_str) { /* this is an old "redirect" rule */
			if (chunk->len + rule->rdr_len > chunk->size - 4)
				goto leave;

			/* add location */
			memcpy(chunk->str + chunk->len, rule->rdr_str, rule->rdr_len);
			chunk->len += rule->rdr_len;
		}
		else {
			/* add location with executing log format */
			chunk->len += build_logline(s, chunk->str + chunk->len, chunk->size - chunk->len, &rule->rdr_fmt);

			/* Check left length */
			if (chunk->len > chunk->size - 4)
				goto leave;
		}
		break;
	}

	if (rule->cookie_len) {
		memcpy(chunk->str + chunk->len, "\r\nSet-Cookie: ", 14);
		chunk->len += 14;
		memcpy(chunk->str + chunk->len, rule->cookie_str, rule->cookie_len);
		chunk->len += rule->cookie_len;
	}

	/* add end of headers and the keep-alive/close status. */
	txn->status = rule->code;
	/* let's log the request time */
	s->logs.tv_request = now;

	if (((!(req->flags & HTTP_MSGF_TE_CHNK) && !req->body_len) || (req->msg_state == HTTP_MSG_DONE)) &&
	    ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL ||
	     (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL)) {
		/* keep-alive possible */
		if (!(req->flags & HTTP_MSGF_VER_11)) {
			if (unlikely(txn->flags & TX_USE_PX_CONN)) {
				memcpy(chunk->str + chunk->len, "\r\nProxy-Connection: keep-alive", 30);
				chunk->len += 30;
			} else {
				memcpy(chunk->str + chunk->len, "\r\nConnection: keep-alive", 24);
				chunk->len += 24;
			}
		}
		memcpy(chunk->str + chunk->len, "\r\n\r\n", 4);
		chunk->len += 4;
		FLT_STRM_CB(s, flt_http_reply(s, txn->status, chunk));
		co_inject(res->chn, chunk->str, chunk->len);
		/* "eat" the request */
		bi_fast_delete(req->chn->buf, req->sov);
		req->next -= req->sov;
		req->sov = 0;
		s->req.analysers = AN_REQ_HTTP_XFER_BODY | (s->req.analysers & AN_REQ_FLT_END);
		s->res.analysers = AN_RES_HTTP_XFER_BODY | (s->res.analysers & AN_RES_FLT_END);
		req->msg_state = HTTP_MSG_CLOSED;
		res->msg_state = HTTP_MSG_DONE;
		/* Trim any possible response */
		res->chn->buf->i = 0;
		res->next = res->sov = 0;
		/* let the server side turn to SI_ST_CLO */
		channel_shutw_now(req->chn);
	} else {
		/* keep-alive not possible */
		if (unlikely(txn->flags & TX_USE_PX_CONN)) {
			memcpy(chunk->str + chunk->len, "\r\nProxy-Connection: close\r\n\r\n", 29);
			chunk->len += 29;
		} else {
			memcpy(chunk->str + chunk->len, "\r\nConnection: close\r\n\r\n", 23);
			chunk->len += 23;
		}
		http_reply_and_close(s, txn->status, chunk);
		req->chn->analysers &= AN_REQ_FLT_END;
	}

	if (!(s->flags & SF_ERR_MASK))
		s->flags |= SF_ERR_LOCAL;
	if (!(s->flags & SF_FINST_MASK))
		s->flags |= SF_FINST_R;

	ret = 1;
 leave:
	free_trash_chunk(chunk);
	return ret;
}
