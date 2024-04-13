step_time(double offset)
{
	llist_t *item;
	double dtime;
	struct timeval tvc, tvn;
	char buf[sizeof("yyyy-mm-dd hh:mm:ss") + /*paranoia:*/ 4];
	time_t tval;

	gettimeofday(&tvc, NULL); /* never fails */
	dtime = tvc.tv_sec + (1.0e-6 * tvc.tv_usec) + offset;
	d_to_tv(dtime, &tvn);
	if (settimeofday(&tvn, NULL) == -1)
		bb_perror_msg_and_die("settimeofday");

	VERB2 {
		tval = tvc.tv_sec;
		strftime_YYYYMMDDHHMMSS(buf, sizeof(buf), &tval);
		bb_error_msg("current time is %s.%06u", buf, (unsigned)tvc.tv_usec);
	}
	tval = tvn.tv_sec;
	strftime_YYYYMMDDHHMMSS(buf, sizeof(buf), &tval);
	bb_error_msg("setting time to %s.%06u (offset %+fs)", buf, (unsigned)tvn.tv_usec, offset);

	/* Correct various fields which contain time-relative values: */

	/* Globals: */
	G.cur_time += offset;
	G.last_update_recv_time += offset;
	G.last_script_run += offset;

	/* p->lastpkt_recv_time, p->next_action_time and such: */
	for (item = G.ntp_peers; item != NULL; item = item->link) {
		peer_t *pp = (peer_t *) item->data;
		reset_peer_stats(pp, offset);
		pp->next_action_time += offset;
		if (pp->p_fd >= 0) {
			/* We wait for reply from this peer too.
			 * But due to step we are doing, reply's data is no longer
			 * useful (in fact, it'll be bogus). Stop waiting for it.
			 */
			close(pp->p_fd);
			pp->p_fd = -1;
			set_next(pp, RETRY_INTERVAL);
		}
	}
}
