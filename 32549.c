int http_response_forward_body(struct session *s, struct channel *res, int an_bit)
{
	struct http_txn *txn = &s->txn;
	struct http_msg *msg = &s->txn.rsp;
	static struct buffer *tmpbuf = NULL;
	int compressing = 0;
	int ret;

	if (unlikely(msg->msg_state < HTTP_MSG_BODY))
		return 0;

	if ((res->flags & (CF_READ_ERROR|CF_READ_TIMEOUT|CF_WRITE_ERROR|CF_WRITE_TIMEOUT)) ||
	    ((res->flags & CF_SHUTW) && (res->to_forward || res->buf->o)) ||
	    !s->req->analysers) {
		/* Output closed while we were sending data. We must abort and
		 * wake the other side up.
		 */
		msg->msg_state = HTTP_MSG_ERROR;
		http_resync_states(s);
		return 1;
	}

	/* in most states, we should abort in case of early close */
	channel_auto_close(res);

	if (msg->sov > 0) {
		/* we have msg->sov which points to the first byte of message
		 * body, and res->buf.p still points to the beginning of the
		 * message. We forward the headers now, as we don't need them
		 * anymore, and we want to flush them.
		 */
		b_adv(res->buf, msg->sov);
		msg->next -= msg->sov;
		msg->sov = 0;

		/* The previous analysers guarantee that the state is somewhere
		 * between MSG_BODY and the first MSG_DATA. So msg->sol and
		 * msg->next are always correct.
		 */
		if (msg->msg_state < HTTP_MSG_CHUNK_SIZE) {
			if (msg->flags & HTTP_MSGF_TE_CHNK)
				msg->msg_state = HTTP_MSG_CHUNK_SIZE;
			else
				msg->msg_state = HTTP_MSG_DATA;
		}
	}

	if (res->to_forward) {
		/* We can't process the buffer's contents yet */
		res->flags |= CF_WAKE_WRITE;
		goto missing_data;
	}

	if (unlikely(s->comp_algo != NULL) && msg->msg_state < HTTP_MSG_TRAILERS) {
		/* We need a compression buffer in the DATA state to put the
		 * output of compressed data, and in CRLF state to let the
		 * TRAILERS state finish the job of removing the trailing CRLF.
		 */
		if (unlikely(tmpbuf == NULL)) {
			/* this is the first time we need the compression buffer */
			tmpbuf = pool_alloc2(pool2_buffer);
			if (tmpbuf == NULL)
				goto aborted_xfer; /* no memory */
		}

		ret = http_compression_buffer_init(s, res->buf, tmpbuf);
		if (ret < 0) {
			res->flags |= CF_WAKE_WRITE;
			goto missing_data; /* not enough spaces in buffers */
		}
		compressing = 1;
	}

	while (1) {
		switch (msg->msg_state - HTTP_MSG_DATA) {
		case HTTP_MSG_DATA - HTTP_MSG_DATA:	/* must still forward */
			/* we may have some pending data starting at res->buf->p */
			if (unlikely(s->comp_algo)) {
				ret = http_compression_buffer_add_data(s, res->buf, tmpbuf);
				if (ret < 0)
					goto aborted_xfer;

				if (msg->chunk_len) {
					/* input empty or output full */
					if (res->buf->i > msg->next)
						res->flags |= CF_WAKE_WRITE;
					goto missing_data;
				}
			}
			else {
				if (msg->chunk_len > res->buf->i - msg->next) {
					/* output full */
					res->flags |= CF_WAKE_WRITE;
					goto missing_data;
				}
				msg->next += msg->chunk_len;
				msg->chunk_len = 0;
			}

			/* nothing left to forward */
			if (msg->flags & HTTP_MSGF_TE_CHNK) {
				msg->msg_state = HTTP_MSG_CHUNK_CRLF;
			} else {
				msg->msg_state = HTTP_MSG_DONE;
				break;
			}
			/* fall through for HTTP_MSG_CHUNK_CRLF */

		case HTTP_MSG_CHUNK_CRLF - HTTP_MSG_DATA:
			/* we want the CRLF after the data */

			ret = http_skip_chunk_crlf(msg);
			if (ret == 0)
				goto missing_data;
			else if (ret < 0) {
				if (msg->err_pos >= 0)
					http_capture_bad_message(&s->be->invalid_rep, s, msg, HTTP_MSG_CHUNK_CRLF, s->fe);
				goto return_bad_res;
			}
			/* we're in MSG_CHUNK_SIZE now, fall through */

		case HTTP_MSG_CHUNK_SIZE - HTTP_MSG_DATA:
			/* read the chunk size and assign it to ->chunk_len, then
			 * set ->next to point to the body and switch to DATA or
			 * TRAILERS state.
			 */

			ret = http_parse_chunk_size(msg);
			if (ret == 0)
				goto missing_data;
			else if (ret < 0) {
				if (msg->err_pos >= 0)
					http_capture_bad_message(&s->be->invalid_rep, s, msg, HTTP_MSG_CHUNK_SIZE, s->fe);
				goto return_bad_res;
			}
			/* otherwise we're in HTTP_MSG_DATA or HTTP_MSG_TRAILERS state */
			break;

		case HTTP_MSG_TRAILERS - HTTP_MSG_DATA:
			if (unlikely(compressing)) {
				/* we need to flush output contents before syncing FSMs */
				http_compression_buffer_end(s, &res->buf, &tmpbuf, 1);
				compressing = 0;
			}

			ret = http_forward_trailers(msg);
			if (ret == 0)
				goto missing_data;
			else if (ret < 0) {
				if (msg->err_pos >= 0)
					http_capture_bad_message(&s->be->invalid_rep, s, msg, HTTP_MSG_TRAILERS, s->fe);
				goto return_bad_res;
			}
			/* we're in HTTP_MSG_DONE now, fall through */

		default:
			/* other states, DONE...TUNNEL */
			if (unlikely(compressing)) {
				/* we need to flush output contents before syncing FSMs */
				http_compression_buffer_end(s, &res->buf, &tmpbuf, 1);
				compressing = 0;
			}

			/* we may have some pending data starting at res->buf->p
			 * such as a last chunk of data or trailers.
			 */
			b_adv(res->buf, msg->next);
			msg->next = 0;

			ret = msg->msg_state;
			/* for keep-alive we don't want to forward closes on DONE */
			if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL ||
			    (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL)
				channel_dont_close(res);

			if (http_resync_states(s)) {
				/* some state changes occurred, maybe the analyser
				 * was disabled too.
				 */
				if (unlikely(msg->msg_state == HTTP_MSG_ERROR)) {
					if (res->flags & CF_SHUTW) {
						/* response errors are most likely due to
						 * the client aborting the transfer.
						 */
						goto aborted_xfer;
					}
					if (msg->err_pos >= 0)
						http_capture_bad_message(&s->be->invalid_rep, s, msg, ret, s->fe);
					goto return_bad_res;
				}
				return 1;
			}
			return 0;
		}
	}

 missing_data:
	/* we may have some pending data starting at res->buf->p */
	if (unlikely(compressing)) {
		http_compression_buffer_end(s, &res->buf, &tmpbuf, msg->msg_state >= HTTP_MSG_TRAILERS);
		compressing = 0;
	}

	if ((s->comp_algo == NULL || msg->msg_state >= HTTP_MSG_TRAILERS)) {
		b_adv(res->buf, msg->next);
		msg->next = 0;
		msg->chunk_len -= channel_forward(res, msg->chunk_len);
	}

	if (res->flags & CF_SHUTW)
		goto aborted_xfer;

	/* stop waiting for data if the input is closed before the end. If the
	 * client side was already closed, it means that the client has aborted,
	 * so we don't want to count this as a server abort. Otherwise it's a
	 * server abort.
	 */
	if (res->flags & CF_SHUTR) {
		if ((s->req->flags & (CF_SHUTR|CF_SHUTW)) == (CF_SHUTR|CF_SHUTW))
			goto aborted_xfer;
		if (!(s->flags & SN_ERR_MASK))
			s->flags |= SN_ERR_SRVCL;
		s->be->be_counters.srv_aborts++;
		if (objt_server(s->target))
			objt_server(s->target)->counters.srv_aborts++;
		goto return_bad_res_stats_ok;
	}

	/* we need to obey the req analyser, so if it leaves, we must too */
	if (!s->req->analysers)
		goto return_bad_res;

	/* When TE: chunked is used, we need to get there again to parse remaining
	 * chunks even if the server has closed, so we don't want to set CF_DONTCLOSE.
	 * Similarly, with keep-alive on the client side, we don't want to forward a
	 * close.
	 */
	if ((msg->flags & HTTP_MSGF_TE_CHNK) || s->comp_algo ||
	    (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL ||
	    (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL)
		channel_dont_close(res);

	/* We know that more data are expected, but we couldn't send more that
	 * what we did. So we always set the CF_EXPECT_MORE flag so that the
	 * system knows it must not set a PUSH on this first part. Interactive
	 * modes are already handled by the stream sock layer. We must not do
	 * this in content-length mode because it could present the MSG_MORE
	 * flag with the last block of forwarded data, which would cause an
	 * additional delay to be observed by the receiver.
	 */
	if ((msg->flags & HTTP_MSGF_TE_CHNK) || s->comp_algo)
		res->flags |= CF_EXPECT_MORE;

	/* the session handler will take care of timeouts and errors */
	return 0;

 return_bad_res: /* let's centralize all bad responses */
	s->be->be_counters.failed_resp++;
	if (objt_server(s->target))
		objt_server(s->target)->counters.failed_resp++;

 return_bad_res_stats_ok:
	if (unlikely(compressing)) {
		http_compression_buffer_end(s, &res->buf, &tmpbuf, msg->msg_state >= HTTP_MSG_TRAILERS);
		compressing = 0;
	}

	/* we may have some pending data starting at res->buf->p */
	if (s->comp_algo == NULL) {
		b_adv(res->buf, msg->next);
		msg->next = 0;
	}

	txn->rsp.msg_state = HTTP_MSG_ERROR;
	/* don't send any error message as we're in the body */
	stream_int_retnclose(res->cons, NULL);
	res->analysers = 0;
	s->req->analysers = 0; /* we're in data phase, we want to abort both directions */
	if (objt_server(s->target))
		health_adjust(objt_server(s->target), HANA_STATUS_HTTP_HDRRSP);

	if (!(s->flags & SN_ERR_MASK))
		s->flags |= SN_ERR_PRXCOND;
	if (!(s->flags & SN_FINST_MASK))
		s->flags |= SN_FINST_D;
	return 0;

 aborted_xfer:
	if (unlikely(compressing)) {
		http_compression_buffer_end(s, &res->buf, &tmpbuf, msg->msg_state >= HTTP_MSG_TRAILERS);
		compressing = 0;
	}

	txn->rsp.msg_state = HTTP_MSG_ERROR;
	/* don't send any error message as we're in the body */
	stream_int_retnclose(res->cons, NULL);
	res->analysers = 0;
	s->req->analysers = 0; /* we're in data phase, we want to abort both directions */

	s->fe->fe_counters.cli_aborts++;
	s->be->be_counters.cli_aborts++;
	if (objt_server(s->target))
		objt_server(s->target)->counters.cli_aborts++;

	if (!(s->flags & SN_ERR_MASK))
		s->flags |= SN_ERR_CLICL;
	if (!(s->flags & SN_FINST_MASK))
		s->flags |= SN_FINST_D;
	return 0;
}
