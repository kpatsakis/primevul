static void si_conn_recv_cb(struct connection *conn)
{
	struct stream_interface *si = conn->owner;
	struct channel *chn = si->ib;
	int ret, max, cur_read;
	int read_poll = MAX_READ_POLL_LOOPS;

	/* stop immediately on errors. Note that we DON'T want to stop on
	 * POLL_ERR, as the poller might report a write error while there
	 * are still data available in the recv buffer. This typically
	 * happens when we send too large a request to a backend server
	 * which rejects it before reading it all.
	 */
	if (conn->flags & CO_FL_ERROR)
		return;

	/* stop here if we reached the end of data */
	if (conn_data_read0_pending(conn))
		goto out_shutdown_r;

	/* maybe we were called immediately after an asynchronous shutr */
	if (chn->flags & CF_SHUTR)
		return;

	cur_read = 0;

	if ((chn->flags & (CF_STREAMER | CF_STREAMER_FAST)) && !chn->buf->o &&
	    global.tune.idle_timer &&
	    (unsigned short)(now_ms - chn->last_read) >= global.tune.idle_timer) {
		/* The buffer was empty and nothing was transferred for more
		 * than one second. This was caused by a pause and not by
		 * congestion. Reset any streaming mode to reduce latency.
		 */
		chn->xfer_small = 0;
		chn->xfer_large = 0;
		chn->flags &= ~(CF_STREAMER | CF_STREAMER_FAST);
	}

	/* First, let's see if we may splice data across the channel without
	 * using a buffer.
	 */
	if (conn->xprt->rcv_pipe &&
	    (chn->pipe || chn->to_forward >= MIN_SPLICE_FORWARD) &&
	    chn->flags & CF_KERN_SPLICING) {
		if (buffer_not_empty(chn->buf)) {
			/* We're embarrassed, there are already data pending in
			 * the buffer and we don't want to have them at two
			 * locations at a time. Let's indicate we need some
			 * place and ask the consumer to hurry.
			 */
			goto abort_splice;
		}

		if (unlikely(chn->pipe == NULL)) {
			if (pipes_used >= global.maxpipes || !(chn->pipe = get_pipe())) {
				chn->flags &= ~CF_KERN_SPLICING;
				goto abort_splice;
			}
		}

		ret = conn->xprt->rcv_pipe(conn, chn->pipe, chn->to_forward);
		if (ret < 0) {
			/* splice not supported on this end, let's disable it */
			chn->flags &= ~CF_KERN_SPLICING;
			goto abort_splice;
		}

		if (ret > 0) {
			if (chn->to_forward != CHN_INFINITE_FORWARD)
				chn->to_forward -= ret;
			chn->total += ret;
			cur_read += ret;
			chn->flags |= CF_READ_PARTIAL;
		}

		if (conn_data_read0_pending(conn))
			goto out_shutdown_r;

		if (conn->flags & CO_FL_ERROR)
			return;

		if (conn->flags & CO_FL_WAIT_ROOM) {
			/* the pipe is full or we have read enough data that it
			 * could soon be full. Let's stop before needing to poll.
			 */
			si->flags |= SI_FL_WAIT_ROOM;
			__conn_data_stop_recv(conn);
		}

		/* splice not possible (anymore), let's go on on standard copy */
	}

 abort_splice:
	if (chn->pipe && unlikely(!chn->pipe->data)) {
		put_pipe(chn->pipe);
		chn->pipe = NULL;
	}

	/* Important note : if we're called with POLL_IN|POLL_HUP, it means the read polling
	 * was enabled, which implies that the recv buffer was not full. So we have a guarantee
	 * that if such an event is not handled above in splice, it will be handled here by
	 * recv().
	 */
	while (!(conn->flags & (CO_FL_ERROR | CO_FL_SOCK_RD_SH | CO_FL_DATA_RD_SH | CO_FL_WAIT_ROOM | CO_FL_HANDSHAKE))) {
		max = bi_avail(chn);

		if (!max) {
			si->flags |= SI_FL_WAIT_ROOM;
			break;
		}

		ret = conn->xprt->rcv_buf(conn, chn->buf, max);
		if (ret <= 0)
			break;

		cur_read += ret;

		/* if we're allowed to directly forward data, we must update ->o */
		if (chn->to_forward && !(chn->flags & (CF_SHUTW|CF_SHUTW_NOW))) {
			unsigned long fwd = ret;
			if (chn->to_forward != CHN_INFINITE_FORWARD) {
				if (fwd > chn->to_forward)
					fwd = chn->to_forward;
				chn->to_forward -= fwd;
			}
			b_adv(chn->buf, fwd);
		}

		chn->flags |= CF_READ_PARTIAL;
		chn->total += ret;

		if (channel_full(chn)) {
			si->flags |= SI_FL_WAIT_ROOM;
			break;
		}

		if ((chn->flags & CF_READ_DONTWAIT) || --read_poll <= 0) {
			si->flags |= SI_FL_WAIT_ROOM;
			__conn_data_stop_recv(conn);
			break;
		}

		/* if too many bytes were missing from last read, it means that
		 * it's pointless trying to read again because the system does
		 * not have them in buffers.
		 */
		if (ret < max) {
			/* if a streamer has read few data, it may be because we
			 * have exhausted system buffers. It's not worth trying
			 * again.
			 */
			if (chn->flags & CF_STREAMER)
				break;

			/* if we read a large block smaller than what we requested,
			 * it's almost certain we'll never get anything more.
			 */
			if (ret >= global.tune.recv_enough)
				break;
		}
	} /* while !flags */

	if (conn->flags & CO_FL_ERROR)
		return;

	if (cur_read) {
		if ((chn->flags & (CF_STREAMER | CF_STREAMER_FAST)) &&
		    (cur_read <= chn->buf->size / 2)) {
			chn->xfer_large = 0;
			chn->xfer_small++;
			if (chn->xfer_small >= 3) {
				/* we have read less than half of the buffer in
				 * one pass, and this happened at least 3 times.
				 * This is definitely not a streamer.
				 */
				chn->flags &= ~(CF_STREAMER | CF_STREAMER_FAST);
			}
			else if (chn->xfer_small >= 2) {
				/* if the buffer has been at least half full twice,
				 * we receive faster than we send, so at least it
				 * is not a "fast streamer".
				 */
				chn->flags &= ~CF_STREAMER_FAST;
			}
		}
		else if (!(chn->flags & CF_STREAMER_FAST) &&
			 (cur_read >= chn->buf->size - global.tune.maxrewrite)) {
			/* we read a full buffer at once */
			chn->xfer_small = 0;
			chn->xfer_large++;
			if (chn->xfer_large >= 3) {
				/* we call this buffer a fast streamer if it manages
				 * to be filled in one call 3 consecutive times.
				 */
				chn->flags |= (CF_STREAMER | CF_STREAMER_FAST);
			}
		}
		else {
			chn->xfer_small = 0;
			chn->xfer_large = 0;
		}
		chn->last_read = now_ms;
	}

	if (conn_data_read0_pending(conn))
		/* connection closed */
		goto out_shutdown_r;

	return;

 out_shutdown_r:
	/* we received a shutdown */
	chn->flags |= CF_READ_NULL;
	if (chn->flags & CF_AUTO_CLOSE)
		channel_shutw_now(chn);
	stream_sock_read0(si);
	conn_data_read0(conn);
	return;
}
