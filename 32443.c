poll_interval(int upper_bound)
{
	unsigned interval, r, mask;
	interval = 1 << G.poll_exp;
	if (interval > upper_bound)
		interval = upper_bound;
	mask = ((interval-1) >> 4) | 1;
	r = rand();
	interval += r & mask; /* ~ random(0..1) * interval/16 */
	VERB4 bb_error_msg("chose poll interval:%u (poll_exp:%d)", interval, G.poll_exp);
	return interval;
}
