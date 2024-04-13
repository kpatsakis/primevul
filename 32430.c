adjust_poll(int count)
{
	G.polladj_count += count;
	if (G.polladj_count > POLLADJ_LIMIT) {
		G.polladj_count = 0;
		if (G.poll_exp < MAXPOLL) {
			G.poll_exp++;
			VERB4 bb_error_msg("polladj: discipline_jitter:%f ++poll_exp=%d",
					G.discipline_jitter, G.poll_exp);
		}
	} else if (G.polladj_count < -POLLADJ_LIMIT || (count < 0 && G.poll_exp > BIGPOLL)) {
		G.polladj_count = 0;
		if (G.poll_exp > MINPOLL) {
			llist_t *item;

			G.poll_exp--;
			/* Correct p->next_action_time in each peer
			 * which waits for sending, so that they send earlier.
			 * Old pp->next_action_time are on the order
			 * of t + (1 << old_poll_exp) + small_random,
			 * we simply need to subtract ~half of that.
			 */
			for (item = G.ntp_peers; item != NULL; item = item->link) {
				peer_t *pp = (peer_t *) item->data;
				if (pp->p_fd < 0)
					pp->next_action_time -= (1 << G.poll_exp);
			}
			VERB4 bb_error_msg("polladj: discipline_jitter:%f --poll_exp=%d",
					G.discipline_jitter, G.poll_exp);
		}
	} else {
		VERB4 bb_error_msg("polladj: count:%d", G.polladj_count);
	}
}
