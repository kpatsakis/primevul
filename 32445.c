reset_peer_stats(peer_t *p, double offset)
{
	int i;
	bool small_ofs = fabs(offset) < STEP_THRESHOLD;

	/* Used to set p->filter_datapoint[i].d_dispersion = MAXDISP
	 * and clear reachable bits, but this proved to be too agressive:
	 * after step (tested with suspending laptop for ~30 secs),
	 * this caused all previous data to be considered invalid,
	 * making us needing to collect full ~8 datapoins per peer
	 * after step in order to start trusting them.
	 * In turn, this was making poll interval decrease even after
	 * step was done. (Poll interval decreases already before step
	 * in this scenario, because we see large offsets and end up with
	 * no good peer to select).
	 */

	for (i = 0; i < NUM_DATAPOINTS; i++) {
		if (small_ofs) {
			p->filter_datapoint[i].d_recv_time += offset;
			if (p->filter_datapoint[i].d_offset != 0) {
				p->filter_datapoint[i].d_offset -= offset;
			}
		} else {
			p->filter_datapoint[i].d_recv_time  = G.cur_time;
			p->filter_datapoint[i].d_offset     = 0;
			/*p->filter_datapoint[i].d_dispersion = MAXDISP;*/
		}
	}
	if (small_ofs) {
		p->lastpkt_recv_time += offset;
	} else {
		/*p->reachable_bits = 0;*/
		p->lastpkt_recv_time = G.cur_time;
	}
	filter_datapoints(p); /* recalc p->filter_xxx */
	VERB6 bb_error_msg("%s->lastpkt_recv_time=%f", p->p_dotted, p->lastpkt_recv_time);
}
