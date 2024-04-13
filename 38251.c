int ping_getfrag(void *from, char *to,
		 int offset, int fraglen, int odd, struct sk_buff *skb)
{
	struct pingfakehdr *pfh = (struct pingfakehdr *)from;

	if (offset == 0) {
		if (fraglen < sizeof(struct icmphdr))
			BUG();
		if (csum_partial_copy_fromiovecend(to + sizeof(struct icmphdr),
			    pfh->iov, 0, fraglen - sizeof(struct icmphdr),
			    &pfh->wcheck))
			return -EFAULT;
	} else if (offset < sizeof(struct icmphdr)) {
			BUG();
	} else {
		if (csum_partial_copy_fromiovecend
				(to, pfh->iov, offset - sizeof(struct icmphdr),
				 fraglen, &pfh->wcheck))
			return -EFAULT;
	}

#if IS_ENABLED(CONFIG_IPV6)
	/* For IPv6, checksum each skb as we go along, as expected by
	 * icmpv6_push_pending_frames. For IPv4, accumulate the checksum in
	 * wcheck, it will be finalized in ping_v4_push_pending_frames.
	 */
	if (pfh->family == AF_INET6) {
		skb->csum = pfh->wcheck;
		skb->ip_summed = CHECKSUM_NONE;
		pfh->wcheck = 0;
	}
#endif

	return 0;
}
