http_msg_forward_chunked_body(struct stream *s, struct http_msg *msg)
{
	struct channel *chn = msg->chn;
	unsigned int chunk;
	int ret;

	/* Here we have the guarantee to be in one of the following state:
	 * HTTP_MSG_DATA, HTTP_MSG_CHUNK_SIZE, HTTP_MSG_CHUNK_CRLF,
	 * HTTP_MSG_TRAILERS or HTTP_MSG_ENDING. */

  switch_states:
	switch (msg->msg_state) {
		case HTTP_MSG_DATA:
			ret = FLT_STRM_DATA_CB(s, chn, flt_http_data(s, msg),
					       /* default_ret */ MIN(msg->chunk_len, chn->buf->i - msg->next),
					       /* on_error    */ goto error);
			msg->next      += ret;
			msg->chunk_len -= ret;
			if (msg->chunk_len) {
				/* input empty or output full */
				if (chn->buf->i > msg->next)
					chn->flags |= CF_WAKE_WRITE;
				goto missing_data_or_waiting;
			}

			/* nothing left to forward for this chunk*/
			msg->msg_state = HTTP_MSG_CHUNK_CRLF;
			/* fall through for HTTP_MSG_CHUNK_CRLF */

		case HTTP_MSG_CHUNK_CRLF:
			/* we want the CRLF after the data */
			ret = h1_skip_chunk_crlf(chn->buf, msg->next, chn->buf->i);
			if (ret == 0)
				goto missing_data_or_waiting;
			if (ret < 0) {
				msg->err_pos = chn->buf->i + ret;
				if (msg->err_pos < 0)
					msg->err_pos += chn->buf->size;
				goto chunk_parsing_error;
			}
			msg->next += ret;
			msg->msg_state = HTTP_MSG_CHUNK_SIZE;
			/* fall through for HTTP_MSG_CHUNK_SIZE */

		case HTTP_MSG_CHUNK_SIZE:
			/* read the chunk size and assign it to ->chunk_len,
			 * then set ->next to point to the body and switch to
			 * DATA or TRAILERS state.
			 */
			ret = h1_parse_chunk_size(chn->buf, msg->next, chn->buf->i, &chunk);
			if (ret == 0)
				goto missing_data_or_waiting;
			if (ret < 0) {
				msg->err_pos = chn->buf->i + ret;
				if (msg->err_pos < 0)
					msg->err_pos += chn->buf->size;
				goto chunk_parsing_error;
			}

			msg->sol = ret;
			msg->next += ret;
			msg->chunk_len = chunk;
			msg->body_len += chunk;

			if (msg->chunk_len) {
				msg->msg_state = HTTP_MSG_DATA;
				goto switch_states;
			}
			msg->msg_state = HTTP_MSG_TRAILERS;
			/* fall through for HTTP_MSG_TRAILERS */

		case HTTP_MSG_TRAILERS:
			ret = http_forward_trailers(msg);
			if (ret < 0)
				goto chunk_parsing_error;
			FLT_STRM_DATA_CB(s, chn, flt_http_chunk_trailers(s, msg),
					 /* default_ret */ 1,
					 /* on_error    */ goto error);
			msg->next += msg->sol;
			if (!ret)
				goto missing_data_or_waiting;
			break;

		case HTTP_MSG_ENDING:
			goto ending;

		default:
			/* This should no happen in this function */
			goto error;
	}

	msg->msg_state = HTTP_MSG_ENDING;
  ending:
	/* we may have some pending data starting at res->buf->p such as a last
	 * chunk of data or trailers. */
	ret = FLT_STRM_DATA_CB(s, chn, flt_http_forward_data(s, msg, msg->next),
			  /* default_ret */ msg->next,
			  /* on_error    */ goto error);
	b_adv(chn->buf, ret);
	msg->next -= ret;
	if (unlikely(!(chn->flags & CF_WROTE_DATA) || msg->sov > 0))
		msg->sov -= ret;
	if (msg->next)
		goto waiting;

	FLT_STRM_DATA_CB(s, chn, flt_http_end(s, msg),
		    /* default_ret */ 1,
		    /* on_error    */ goto error,
		    /* on_wait     */ goto waiting);
	msg->msg_state = HTTP_MSG_DONE;
	return 1;

  missing_data_or_waiting:
	/* we may have some pending data starting at chn->buf->p */
	ret = FLT_STRM_DATA_CB(s, chn, flt_http_forward_data(s, msg, msg->next),
			  /* default_ret */ msg->next,
			  /* on_error    */ goto error);
	b_adv(chn->buf, ret);
	msg->next -= ret;
	if (!(chn->flags & CF_WROTE_DATA) || msg->sov > 0)
		msg->sov -= ret;
	if (!HAS_DATA_FILTERS(s, chn))
		msg->chunk_len -= channel_forward(chn, msg->chunk_len);
  waiting:
	return 0;

  chunk_parsing_error:
	if (msg->err_pos >= 0) {
		if (chn->flags & CF_ISRESP)
			http_capture_bad_message(s->be, &s->be->invalid_rep, s, msg,
						 msg->msg_state, strm_fe(s));
		else
			http_capture_bad_message(strm_fe(s), &strm_fe(s)->invalid_req, s,
						 msg, msg->msg_state, s->be);
	}
  error:
	return -1;
}
