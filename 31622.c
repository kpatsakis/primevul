int http_sync_res_state(struct stream *s)
{
	struct channel *chn = &s->res;
	struct http_txn *txn = s->txn;
	unsigned int old_flags = chn->flags;
	unsigned int old_state = txn->rsp.msg_state;

	if (unlikely(txn->rsp.msg_state < HTTP_MSG_DONE))
		return 0;

	if (txn->rsp.msg_state == HTTP_MSG_DONE) {
		/* In theory, we don't need to read anymore, but we must
		 * still monitor the server connection for a possible close
		 * while the request is being uploaded, so we don't disable
		 * reading.
		 */
		/* channel_dont_read(chn); */

		if (txn->req.msg_state == HTTP_MSG_ERROR)
			goto wait_other_side;

		if (txn->req.msg_state < HTTP_MSG_DONE) {
			/* The client seems to still be sending data, probably
			 * because we got an error response during an upload.
			 * We have the choice of either breaking the connection
			 * or letting it pass through. Let's do the later.
			 */
			goto wait_other_side;
		}

		/* When we get here, it means that both the request and the
		 * response have finished receiving. Depending on the connection
		 * mode, we'll have to wait for the last bytes to leave in either
		 * direction, and sometimes for a close to be effective.
		 */

		if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL) {
			/* Server-close mode : shut read and wait for the request
			 * side to close its output buffer. The caller will detect
			 * when we're in DONE and the other is in CLOSED and will
			 * catch that for the final cleanup.
			 */
			if (!(chn->flags & (CF_SHUTR|CF_SHUTR_NOW)))
				channel_shutr_now(chn);
		}
		else if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_CLO) {
			/* Option forceclose is set, or either side wants to close,
			 * let's enforce it now that we're not expecting any new
			 * data to come. The caller knows the stream is complete
			 * once both states are CLOSED.
			 */
			if (!(chn->flags & (CF_SHUTW|CF_SHUTW_NOW))) {
				channel_shutr_now(chn);
				channel_shutw_now(chn);
			}
		}
		else {
			/* The last possible modes are keep-alive and tunnel. Tunnel will
			 * need to forward remaining data. Keep-alive will need to monitor
			 * for connection closing.
			 */
			channel_auto_read(chn);
			chn->flags |= CF_NEVER_WAIT;
			if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_TUN)
				txn->rsp.msg_state = HTTP_MSG_TUNNEL;
		}

		goto check_channel_flags;
	}

	if (txn->rsp.msg_state == HTTP_MSG_CLOSING) {
	http_msg_closing:
		/* nothing else to forward, just waiting for the output buffer
		 * to be empty and for the shutw_now to take effect.
		 */
		if (channel_is_empty(chn)) {
			txn->rsp.msg_state = HTTP_MSG_CLOSED;
			goto http_msg_closed;
		}
		else if (chn->flags & CF_SHUTW) {
			txn->rsp.err_state = txn->rsp.msg_state;
			txn->rsp.msg_state = HTTP_MSG_ERROR;
			HA_ATOMIC_ADD(&s->be->be_counters.cli_aborts, 1);
			if (objt_server(s->target))
				HA_ATOMIC_ADD(&objt_server(s->target)->counters.cli_aborts, 1);
		}
		goto wait_other_side;
	}

	if (txn->rsp.msg_state == HTTP_MSG_CLOSED) {
	http_msg_closed:
		/* drop any pending data */
		channel_truncate(chn);
		channel_auto_close(chn);
		channel_auto_read(chn);
		goto wait_other_side;
	}

 check_channel_flags:
	/* Here, we are in HTTP_MSG_DONE or HTTP_MSG_TUNNEL */
	if (chn->flags & (CF_SHUTW|CF_SHUTW_NOW)) {
		/* if we've just closed an output, let's switch */
		txn->rsp.msg_state = HTTP_MSG_CLOSING;
		goto http_msg_closing;
	}

 wait_other_side:
	/* We force the response to leave immediately if we're waiting for the
	 * other side, since there is no pending shutdown to push it out.
	 */
	if (!channel_is_empty(chn))
		chn->flags |= CF_SEND_DONTWAIT;
	return txn->rsp.msg_state != old_state || chn->flags != old_flags;
}
