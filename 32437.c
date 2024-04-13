filter_datapoints(peer_t *p)
{
	int i, idx;
	double sum, wavg;
	datapoint_t *fdp;

#if 0
/* Simulations have shown that use of *averaged* offset for p->filter_offset
 * is in fact worse than simply using last received one: with large poll intervals
 * (>= 2048) averaging code uses offset values which are outdated by hours,
 * and time/frequency correction goes totally wrong when fed essentially bogus offsets.
 */
	int got_newest;
	double minoff, maxoff, w;
	double x = x; /* for compiler */
	double oldest_off = oldest_off;
	double oldest_age = oldest_age;
	double newest_off = newest_off;
	double newest_age = newest_age;

	fdp = p->filter_datapoint;

	minoff = maxoff = fdp[0].d_offset;
	for (i = 1; i < NUM_DATAPOINTS; i++) {
		if (minoff > fdp[i].d_offset)
			minoff = fdp[i].d_offset;
		if (maxoff < fdp[i].d_offset)
			maxoff = fdp[i].d_offset;
	}

	idx = p->datapoint_idx; /* most recent datapoint's index */
	/* Average offset:
	 * Drop two outliers and take weighted average of the rest:
	 * most_recent/2 + older1/4 + older2/8 ... + older5/32 + older6/32
	 * we use older6/32, not older6/64 since sum of weights should be 1:
	 * 1/2 + 1/4 + 1/8 + 1/16 + 1/32 + 1/32 = 1
	 */
	wavg = 0;
	w = 0.5;
	/*                     n-1
	 *                     ---    dispersion(i)
	 * filter_dispersion =  \     -------------
	 *                      /       (i+1)
	 *                     ---     2
	 *                     i=0
	 */
	got_newest = 0;
	sum = 0;
	for (i = 0; i < NUM_DATAPOINTS; i++) {
		VERB5 {
			bb_error_msg("datapoint[%d]: off:%f disp:%f(%f) age:%f%s",
				i,
				fdp[idx].d_offset,
				fdp[idx].d_dispersion, dispersion(&fdp[idx]),
				G.cur_time - fdp[idx].d_recv_time,
				(minoff == fdp[idx].d_offset || maxoff == fdp[idx].d_offset)
					? " (outlier by offset)" : ""
			);
		}

		sum += dispersion(&fdp[idx]) / (2 << i);

		if (minoff == fdp[idx].d_offset) {
			minoff -= 1; /* so that we don't match it ever again */
		} else
		if (maxoff == fdp[idx].d_offset) {
			maxoff += 1;
		} else {
			oldest_off = fdp[idx].d_offset;
			oldest_age = G.cur_time - fdp[idx].d_recv_time;
			if (!got_newest) {
				got_newest = 1;
				newest_off = oldest_off;
				newest_age = oldest_age;
			}
			x = oldest_off * w;
			wavg += x;
			w /= 2;
		}

		idx = (idx - 1) & (NUM_DATAPOINTS - 1);
	}
	p->filter_dispersion = sum;
	wavg += x; /* add another older6/64 to form older6/32 */
	/* Fix systematic underestimation with large poll intervals.
	 * Imagine that we still have a bit of uncorrected drift,
	 * and poll interval is big (say, 100 sec). Offsets form a progression:
	 * 0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 - 0.7 is most recent.
	 * The algorithm above drops 0.0 and 0.7 as outliers,
	 * and then we have this estimation, ~25% off from 0.7:
	 * 0.1/32 + 0.2/32 + 0.3/16 + 0.4/8 + 0.5/4 + 0.6/2 = 0.503125
	 */
	x = oldest_age - newest_age;
	if (x != 0) {
		x = newest_age / x; /* in above example, 100 / (600 - 100) */
		if (x < 1) { /* paranoia check */
			x = (newest_off - oldest_off) * x; /* 0.5 * 100/500 = 0.1 */
			wavg += x;
		}
	}
	p->filter_offset = wavg;

#else

	fdp = p->filter_datapoint;
	idx = p->datapoint_idx; /* most recent datapoint's index */

	/* filter_offset: simply use the most recent value */
	p->filter_offset = fdp[idx].d_offset;

	/*                     n-1
	 *                     ---    dispersion(i)
	 * filter_dispersion =  \     -------------
	 *                      /       (i+1)
	 *                     ---     2
	 *                     i=0
	 */
	wavg = 0;
	sum = 0;
	for (i = 0; i < NUM_DATAPOINTS; i++) {
		sum += dispersion(&fdp[idx]) / (2 << i);
		wavg += fdp[idx].d_offset;
		idx = (idx - 1) & (NUM_DATAPOINTS - 1);
	}
	wavg /= NUM_DATAPOINTS;
	p->filter_dispersion = sum;
#endif

	/*                  +-----                 -----+ ^ 1/2
	 *                  |       n-1                 |
	 *                  |       ---                 |
	 *                  |  1    \                2  |
	 * filter_jitter =  | --- * /  (avg-offset_j)   |
	 *                  |  n    ---                 |
	 *                  |       j=0                 |
	 *                  +-----                 -----+
	 * where n is the number of valid datapoints in the filter (n > 1);
	 * if filter_jitter < precision then filter_jitter = precision
	 */
	sum = 0;
	for (i = 0; i < NUM_DATAPOINTS; i++) {
		sum += SQUARE(wavg - fdp[i].d_offset);
	}
	sum = SQRT(sum / NUM_DATAPOINTS);
	p->filter_jitter = sum > G_precision_sec ? sum : G_precision_sec;

	VERB4 bb_error_msg("filter offset:%+f disp:%f jitter:%f",
			p->filter_offset,
			p->filter_dispersion,
			p->filter_jitter);
}
