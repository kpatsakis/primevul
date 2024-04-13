root_distance(peer_t *p)
{
	/* The root synchronization distance is the maximum error due to
	 * all causes of the local clock relative to the primary server.
	 * It is defined as half the total delay plus total dispersion
	 * plus peer jitter.
	 */
	return MAXD(MINDISP, p->lastpkt_rootdelay + p->lastpkt_delay) / 2
		+ p->lastpkt_rootdisp
		+ p->filter_dispersion
		+ FREQ_TOLERANCE * (G.cur_time - p->lastpkt_recv_time)
		+ p->filter_jitter;
}
