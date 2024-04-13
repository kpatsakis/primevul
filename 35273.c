static void tcp_illinois_acked(struct sock *sk, u32 pkts_acked, s32 rtt)
{
	struct illinois *ca = inet_csk_ca(sk);

	ca->acked = pkts_acked;

	/* dup ack, no rtt sample */
	if (rtt < 0)
		return;

	/* ignore bogus values, this prevents wraparound in alpha math */
	if (rtt > RTT_MAX)
		rtt = RTT_MAX;

	/* keep track of minimum RTT seen so far */
	if (ca->base_rtt > rtt)
		ca->base_rtt = rtt;

	/* and max */
	if (ca->max_rtt < rtt)
		ca->max_rtt = rtt;

	++ca->cnt_rtt;
	ca->sum_rtt += rtt;
}
