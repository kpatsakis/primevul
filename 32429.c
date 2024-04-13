add_peers(const char *s)
{
	llist_t *item;
	peer_t *p;

	p = xzalloc(sizeof(*p) + strlen(s));
	strcpy(p->p_hostname, s);
	resolve_peer_hostname(p, /*loop_on_fail=*/ 1);

	/* Names like N.<country2chars>.pool.ntp.org are randomly resolved
	 * to a pool of machines. Sometimes different N's resolve to the same IP.
	 * It is not useful to have two peers with same IP. We skip duplicates.
	 */
	for (item = G.ntp_peers; item != NULL; item = item->link) {
		peer_t *pp = (peer_t *) item->data;
		if (strcmp(p->p_dotted, pp->p_dotted) == 0) {
			bb_error_msg("duplicate peer %s (%s)", s, p->p_dotted);
			free(p->p_lsa);
			free(p->p_dotted);
			free(p);
			return;
		}
	}

	p->p_fd = -1;
	p->p_xmt_msg.m_status = MODE_CLIENT | (NTP_VERSION << 3);
	p->next_action_time = G.cur_time; /* = set_next(p, 0); */
	reset_peer_stats(p, STEP_THRESHOLD);

	llist_add_to(&G.ntp_peers, p);
	G.peer_cnt++;
}
