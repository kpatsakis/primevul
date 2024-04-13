static int br_multicast_ipv6_rcv(struct net_bridge *br,
				 struct net_bridge_port *port,
				 struct sk_buff *skb)
{
	struct sk_buff *skb2;
	const struct ipv6hdr *ip6h;
	u8 icmp6_type;
	u8 nexthdr;
	__be16 frag_off;
	unsigned int len;
	int offset;
	int err;
	u16 vid = 0;

	if (!pskb_may_pull(skb, sizeof(*ip6h)))
		return -EINVAL;

	ip6h = ipv6_hdr(skb);

	/*
	 * We're interested in MLD messages only.
	 *  - Version is 6
	 *  - MLD has always Router Alert hop-by-hop option
	 *  - But we do not support jumbrograms.
	 */
	if (ip6h->version != 6 ||
	    ip6h->nexthdr != IPPROTO_HOPOPTS ||
	    ip6h->payload_len == 0)
		return 0;

	len = ntohs(ip6h->payload_len) + sizeof(*ip6h);
	if (skb->len < len)
		return -EINVAL;

	nexthdr = ip6h->nexthdr;
	offset = ipv6_skip_exthdr(skb, sizeof(*ip6h), &nexthdr, &frag_off);

	if (offset < 0 || nexthdr != IPPROTO_ICMPV6)
		return 0;

	/* Okay, we found ICMPv6 header */
	skb2 = skb_clone(skb, GFP_ATOMIC);
	if (!skb2)
		return -ENOMEM;

	err = -EINVAL;
	if (!pskb_may_pull(skb2, offset + sizeof(struct icmp6hdr)))
		goto out;

	len -= offset - skb_network_offset(skb2);

	__skb_pull(skb2, offset);
	skb_reset_transport_header(skb2);
	skb_postpull_rcsum(skb2, skb_network_header(skb2),
			   skb_network_header_len(skb2));

	icmp6_type = icmp6_hdr(skb2)->icmp6_type;

	switch (icmp6_type) {
	case ICMPV6_MGM_QUERY:
	case ICMPV6_MGM_REPORT:
	case ICMPV6_MGM_REDUCTION:
	case ICMPV6_MLD2_REPORT:
		break;
	default:
		err = 0;
		goto out;
	}

	/* Okay, we found MLD message. Check further. */
	if (skb2->len > len) {
		err = pskb_trim_rcsum(skb2, len);
		if (err)
			goto out;
		err = -EINVAL;
	}

	ip6h = ipv6_hdr(skb2);

	switch (skb2->ip_summed) {
	case CHECKSUM_COMPLETE:
		if (!csum_ipv6_magic(&ip6h->saddr, &ip6h->daddr, skb2->len,
					IPPROTO_ICMPV6, skb2->csum))
			break;
		/*FALLTHROUGH*/
	case CHECKSUM_NONE:
		skb2->csum = ~csum_unfold(csum_ipv6_magic(&ip6h->saddr,
							&ip6h->daddr,
							skb2->len,
							IPPROTO_ICMPV6, 0));
		if (__skb_checksum_complete(skb2))
			goto out;
	}

	err = 0;

	br_vlan_get_tag(skb, &vid);
	BR_INPUT_SKB_CB(skb)->igmp = 1;

	switch (icmp6_type) {
	case ICMPV6_MGM_REPORT:
	    {
		struct mld_msg *mld;
		if (!pskb_may_pull(skb2, sizeof(*mld))) {
			err = -EINVAL;
			goto out;
		}
		mld = (struct mld_msg *)skb_transport_header(skb2);
		BR_INPUT_SKB_CB(skb)->mrouters_only = 1;
		err = br_ip6_multicast_add_group(br, port, &mld->mld_mca, vid);
		break;
	    }
	case ICMPV6_MLD2_REPORT:
		err = br_ip6_multicast_mld2_report(br, port, skb2);
		break;
	case ICMPV6_MGM_QUERY:
		err = br_ip6_multicast_query(br, port, skb2);
		break;
	case ICMPV6_MGM_REDUCTION:
	    {
		struct mld_msg *mld;
		if (!pskb_may_pull(skb2, sizeof(*mld))) {
			err = -EINVAL;
			goto out;
		}
		mld = (struct mld_msg *)skb_transport_header(skb2);
		br_ip6_multicast_leave_group(br, port, &mld->mld_mca, vid);
	    }
	}

out:
	kfree_skb(skb2);
	return err;
}
