void http_resync_states(struct stream *s)
{
	struct http_txn *txn = s->txn;
#ifdef DEBUG_FULL
	int old_req_state = txn->req.msg_state;
	int old_res_state = txn->rsp.msg_state;
#endif

	http_sync_req_state(s);
	while (1) {
		if (!http_sync_res_state(s))
			break;
		if (!http_sync_req_state(s))
			break;
	}

	DPRINTF(stderr,"[%u] %s: stream=%p old=%s,%s cur=%s,%s "
		"req->analysers=0x%08x res->analysers=0x%08x\n",
		now_ms, __FUNCTION__, s,
		h1_msg_state_str(old_req_state), h1_msg_state_str(old_res_state),
		h1_msg_state_str(txn->req.msg_state), h1_msg_state_str(txn->rsp.msg_state),
		s->req.analysers, s->res.analysers);


	/* OK, both state machines agree on a compatible state.
	 * There are a few cases we're interested in :
	 *  - HTTP_MSG_CLOSED on both sides means we've reached the end in both
	 *    directions, so let's simply disable both analysers.
	 *  - HTTP_MSG_CLOSED on the response only or HTTP_MSG_ERROR on either
	 *    means we must abort the request.
	 *  - HTTP_MSG_TUNNEL on either means we have to disable analyser on
	 *    corresponding channel.
	 *  - HTTP_MSG_DONE or HTTP_MSG_CLOSED on the request and HTTP_MSG_DONE
	 *    on the response with server-close mode means we've completed one
	 *    request and we must re-initialize the server connection.
	 */
	if (txn->req.msg_state == HTTP_MSG_CLOSED &&
	    txn->rsp.msg_state == HTTP_MSG_CLOSED) {
		s->req.analysers &= AN_REQ_FLT_END;
		channel_auto_close(&s->req);
		channel_auto_read(&s->req);
		s->res.analysers &= AN_RES_FLT_END;
		channel_auto_close(&s->res);
		channel_auto_read(&s->res);
	}
	else if (txn->rsp.msg_state == HTTP_MSG_CLOSED ||
		 txn->rsp.msg_state == HTTP_MSG_ERROR  ||
		 txn->req.msg_state == HTTP_MSG_ERROR) {
		s->res.analysers &= AN_RES_FLT_END;
		channel_auto_close(&s->res);
		channel_auto_read(&s->res);
		s->req.analysers &= AN_REQ_FLT_END;
		channel_abort(&s->req);
		channel_auto_close(&s->req);
		channel_auto_read(&s->req);
		channel_truncate(&s->req);
	}
	else if (txn->req.msg_state == HTTP_MSG_TUNNEL ||
		 txn->rsp.msg_state == HTTP_MSG_TUNNEL) {
		if (txn->req.msg_state == HTTP_MSG_TUNNEL) {
			s->req.analysers &= AN_REQ_FLT_END;
			if (HAS_REQ_DATA_FILTERS(s))
				s->req.analysers |= AN_REQ_FLT_XFER_DATA;
		}
		if (txn->rsp.msg_state == HTTP_MSG_TUNNEL) {
			s->res.analysers &= AN_RES_FLT_END;
			if (HAS_RSP_DATA_FILTERS(s))
				s->res.analysers |= AN_RES_FLT_XFER_DATA;
		}
		channel_auto_close(&s->req);
		channel_auto_read(&s->req);
		channel_auto_close(&s->res);
		channel_auto_read(&s->res);
	}
	else if ((txn->req.msg_state == HTTP_MSG_DONE ||
		  txn->req.msg_state == HTTP_MSG_CLOSED) &&
		 txn->rsp.msg_state == HTTP_MSG_DONE &&
		 ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL ||
		  (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL)) {
		/* server-close/keep-alive: terminate this transaction,
		 * possibly killing the server connection and reinitialize
		 * a fresh-new transaction, but only once we're sure there's
		 * enough room in the request and response buffer to process
		 * another request. They must not hold any pending output data
		 * and the response buffer must realigned
		 * (realign is done is http_end_txn_clean_session).
		 */
		if (s->req.buf->o)
			s->req.flags |= CF_WAKE_WRITE;
		else if (s->res.buf->o)
			s->res.flags |= CF_WAKE_WRITE;
		else {
			s->req.analysers = AN_REQ_FLT_END;
			s->res.analysers = AN_RES_FLT_END;
			txn->flags |= TX_WAIT_CLEANUP;
		}
	}
}
