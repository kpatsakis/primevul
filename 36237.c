static int igmpv3_sendpack(struct sk_buff *skb)
{
	struct igmphdr *pig = igmp_hdr(skb);
	const int igmplen = skb->tail - skb->transport_header;

	pig->csum = ip_compute_csum(igmp_hdr(skb), igmplen);

	return ip_local_out(skb);
}
