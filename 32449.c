select_and_cluster(void)
{
	peer_t     *p;
	llist_t    *item;
	int        i, j;
	int        size = 3 * G.peer_cnt;
	/* for selection algorithm */
	point_t    point[size];
	unsigned   num_points, num_candidates;
	double     low, high;
	unsigned   num_falsetickers;
	/* for cluster algorithm */
	survivor_t survivor[size];
	unsigned   num_survivors;

	/* Selection */

	num_points = 0;
	item = G.ntp_peers;
	while (item != NULL) {
		double rd, offset;

		p = (peer_t *) item->data;
		rd = root_distance(p);
		offset = p->filter_offset;
		if (!fit(p, rd)) {
			item = item->link;
			continue;
		}

		VERB5 bb_error_msg("interval: [%f %f %f] %s",
				offset - rd,
				offset,
				offset + rd,
				p->p_dotted
		);
		point[num_points].p = p;
		point[num_points].type = -1;
		point[num_points].edge = offset - rd;
		point[num_points].opt_rd = rd;
		num_points++;
		point[num_points].p = p;
		point[num_points].type = 0;
		point[num_points].edge = offset;
		point[num_points].opt_rd = rd;
		num_points++;
		point[num_points].p = p;
		point[num_points].type = 1;
		point[num_points].edge = offset + rd;
		point[num_points].opt_rd = rd;
		num_points++;
		item = item->link;
	}
	num_candidates = num_points / 3;
	if (num_candidates == 0) {
		VERB3 bb_error_msg("no valid datapoints%s", ", no peer selected");
		return NULL;
	}
	qsort(point, num_points, sizeof(point[0]), compare_point_edge);

	/* Start with the assumption that there are no falsetickers.
	 * Attempt to find a nonempty intersection interval containing
	 * the midpoints of all truechimers.
	 * If a nonempty interval cannot be found, increase the number
	 * of assumed falsetickers by one and try again.
	 * If a nonempty interval is found and the number of falsetickers
	 * is less than the number of truechimers, a majority has been found
	 * and the midpoint of each truechimer represents
	 * the candidates available to the cluster algorithm.
	 */
	num_falsetickers = 0;
	while (1) {
		int c;
		unsigned num_midpoints = 0;

		low = 1 << 9;
		high = - (1 << 9);
		c = 0;
		for (i = 0; i < num_points; i++) {
			/* We want to do:
			 * if (point[i].type == -1) c++;
			 * if (point[i].type == 1) c--;
			 * and it's simpler to do it this way:
			 */
			c -= point[i].type;
			if (c >= num_candidates - num_falsetickers) {
				/* If it was c++ and it got big enough... */
				low = point[i].edge;
				break;
			}
			if (point[i].type == 0)
				num_midpoints++;
		}
		c = 0;
		for (i = num_points-1; i >= 0; i--) {
			c += point[i].type;
			if (c >= num_candidates - num_falsetickers) {
				high = point[i].edge;
				break;
			}
			if (point[i].type == 0)
				num_midpoints++;
		}
		/* If the number of midpoints is greater than the number
		 * of allowed falsetickers, the intersection contains at
		 * least one truechimer with no midpoint - bad.
		 * Also, interval should be nonempty.
		 */
		if (num_midpoints <= num_falsetickers && low < high)
			break;
		num_falsetickers++;
		if (num_falsetickers * 2 >= num_candidates) {
			VERB3 bb_error_msg("falsetickers:%d, candidates:%d%s",
					num_falsetickers, num_candidates,
					", no peer selected");
			return NULL;
		}
	}
	VERB4 bb_error_msg("selected interval: [%f, %f]; candidates:%d falsetickers:%d",
			low, high, num_candidates, num_falsetickers);

	/* Clustering */

	/* Construct a list of survivors (p, metric)
	 * from the chime list, where metric is dominated
	 * first by stratum and then by root distance.
	 * All other things being equal, this is the order of preference.
	 */
	num_survivors = 0;
	for (i = 0; i < num_points; i++) {
		if (point[i].edge < low || point[i].edge > high)
			continue;
		p = point[i].p;
		survivor[num_survivors].p = p;
		/* x.opt_rd == root_distance(p); */
		survivor[num_survivors].metric = MAXDIST * p->lastpkt_stratum + point[i].opt_rd;
		VERB5 bb_error_msg("survivor[%d] metric:%f peer:%s",
			num_survivors, survivor[num_survivors].metric, p->p_dotted);
		num_survivors++;
	}
	/* There must be at least MIN_SELECTED survivors to satisfy the
	 * correctness assertions. Ordinarily, the Byzantine criteria
	 * require four survivors, but for the demonstration here, one
	 * is acceptable.
	 */
	if (num_survivors < MIN_SELECTED) {
		VERB3 bb_error_msg("survivors:%d%s",
				num_survivors,
				", no peer selected");
		return NULL;
	}

	qsort(survivor, num_survivors, sizeof(survivor[0]), compare_survivor_metric);

	/* For each association p in turn, calculate the selection
	 * jitter p->sjitter as the square root of the sum of squares
	 * (p->offset - q->offset) over all q associations. The idea is
	 * to repeatedly discard the survivor with maximum selection
	 * jitter until a termination condition is met.
	 */
	while (1) {
		unsigned max_idx = max_idx;
		double max_selection_jitter = max_selection_jitter;
		double min_jitter = min_jitter;

		if (num_survivors <= MIN_CLUSTERED) {
			VERB4 bb_error_msg("num_survivors %d <= %d, not discarding more",
					num_survivors, MIN_CLUSTERED);
			break;
		}

		/* To make sure a few survivors are left
		 * for the clustering algorithm to chew on,
		 * we stop if the number of survivors
		 * is less than or equal to MIN_CLUSTERED (3).
		 */
		for (i = 0; i < num_survivors; i++) {
			double selection_jitter_sq;

			p = survivor[i].p;
			if (i == 0 || p->filter_jitter < min_jitter)
				min_jitter = p->filter_jitter;

			selection_jitter_sq = 0;
			for (j = 0; j < num_survivors; j++) {
				peer_t *q = survivor[j].p;
				selection_jitter_sq += SQUARE(p->filter_offset - q->filter_offset);
			}
			if (i == 0 || selection_jitter_sq > max_selection_jitter) {
				max_selection_jitter = selection_jitter_sq;
				max_idx = i;
			}
			VERB6 bb_error_msg("survivor %d selection_jitter^2:%f",
					i, selection_jitter_sq);
		}
		max_selection_jitter = SQRT(max_selection_jitter / num_survivors);
		VERB5 bb_error_msg("max_selection_jitter (at %d):%f min_jitter:%f",
				max_idx, max_selection_jitter, min_jitter);

		/* If the maximum selection jitter is less than the
		 * minimum peer jitter, then tossing out more survivors
		 * will not lower the minimum peer jitter, so we might
		 * as well stop.
		 */
		if (max_selection_jitter < min_jitter) {
			VERB4 bb_error_msg("max_selection_jitter:%f < min_jitter:%f, num_survivors:%d, not discarding more",
					max_selection_jitter, min_jitter, num_survivors);
			break;
		}

		/* Delete survivor[max_idx] from the list
		 * and go around again.
		 */
		VERB6 bb_error_msg("dropping survivor %d", max_idx);
		num_survivors--;
		while (max_idx < num_survivors) {
			survivor[max_idx] = survivor[max_idx + 1];
			max_idx++;
		}
	}

	if (0) {
		/* Combine the offsets of the clustering algorithm survivors
		 * using a weighted average with weight determined by the root
		 * distance. Compute the selection jitter as the weighted RMS
		 * difference between the first survivor and the remaining
		 * survivors. In some cases the inherent clock jitter can be
		 * reduced by not using this algorithm, especially when frequent
		 * clockhopping is involved. bbox: thus we don't do it.
		 */
		double x, y, z, w;
		y = z = w = 0;
		for (i = 0; i < num_survivors; i++) {
			p = survivor[i].p;
			x = root_distance(p);
			y += 1 / x;
			z += p->filter_offset / x;
			w += SQUARE(p->filter_offset - survivor[0].p->filter_offset) / x;
		}
	}

	/* Pick the best clock. If the old system peer is on the list
	 * and at the same stratum as the first survivor on the list,
	 * then don't do a clock hop. Otherwise, select the first
	 * survivor on the list as the new system peer.
	 */
	p = survivor[0].p;
	if (G.last_update_peer
	 && G.last_update_peer->lastpkt_stratum <= p->lastpkt_stratum
	) {
		/* Starting from 1 is ok here */
		for (i = 1; i < num_survivors; i++) {
			if (G.last_update_peer == survivor[i].p) {
				VERB5 bb_error_msg("keeping old synced peer");
				p = G.last_update_peer;
				goto keep_old;
			}
		}
	}
	G.last_update_peer = p;
 keep_old:
	VERB4 bb_error_msg("selected peer %s filter_offset:%+f age:%f",
			p->p_dotted,
			p->filter_offset,
			G.cur_time - p->lastpkt_recv_time
	);
	return p;
}
