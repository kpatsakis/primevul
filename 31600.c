http_msg_forward_body(struct stream *s, struct http_msg *msg)
{
	struct channel *chn = msg->chn;
	int ret;

	/* Here we have the guarantee to be in HTTP_MSG_DATA or HTTP_MSG_ENDING state */

	if (msg->msg_state == HTTP_MSG_ENDING)
		goto ending;

	/* Neither content-length, nor transfer-encoding was found, so we must
	 * read the body until the server connection is closed. In that case, we
	 * eat data as they come. Of course, this happens for response only. */
	if (!(msg->flags & HTTP_MSGF_XFER_LEN)) {
		unsigned long long len = (chn->buf->i - msg->next);
		msg->chunk_len += len;
		msg->body_len  += len;
	}
	ret = FLT_STRM_DATA_CB(s, chn, flt_http_data(s, msg),
			       /* default_ret */ MIN(msg->chunk_len, chn->buf->i - msg->next),
			       /* on_error    */ goto error);
	msg->next     += ret;
	msg->chunk_len -= ret;
	if (msg->chunk_len) {
		/* input empty or output full */
		if (chn->buf->i > msg->next)
			chn->flags |= CF_WAKE_WRITE;
		goto missing_data_or_waiting;
	}

	/* This check can only be true for a response. HTTP_MSGF_XFER_LEN is
	 * always set for a request. */
	if (!(msg->flags & HTTP_MSGF_XFER_LEN)) {
		/* The server still sending data that should be filtered */
		if (!(chn->flags & CF_SHUTR) && HAS_DATA_FILTERS(s, chn))
			goto missing_data_or_waiting;
		msg->msg_state = HTTP_MSG_TUNNEL;
		goto ending;
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
	if (msg->msg_state == HTTP_MSG_ENDING)
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
  error:
	return -1;
}
