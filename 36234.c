static struct sk_buff *igmpv3_newpack(struct net_device *dev, int size)
{
	struct sk_buff *skb;
	struct rtable *rt;
	struct iphdr *pip;
	struct igmpv3_report *pig;
	struct net *net = dev_net(dev);
	struct flowi4 fl4;
	int hlen = LL_RESERVED_SPACE(dev);
	int tlen = dev->needed_tailroom;

	while (1) {
		skb = alloc_skb(size + hlen + tlen,
				GFP_ATOMIC | __GFP_NOWARN);
		if (skb)
			break;
		size >>= 1;
		if (size < 256)
			return NULL;
	}
	igmp_skb_size(skb) = size;

	rt = ip_route_output_ports(net, &fl4, NULL, IGMPV3_ALL_MCR, 0,
				   0, 0,
				   IPPROTO_IGMP, 0, dev->ifindex);
	if (IS_ERR(rt)) {
		kfree_skb(skb);
		return NULL;
	}

	skb_dst_set(skb, &rt->dst);
	skb->dev = dev;

	skb_reserve(skb, hlen);

	skb_reset_network_header(skb);
	pip = ip_hdr(skb);
	skb_put(skb, sizeof(struct iphdr) + 4);

	pip->version  = 4;
	pip->ihl      = (sizeof(struct iphdr)+4)>>2;
	pip->tos      = 0xc0;
	pip->frag_off = htons(IP_DF);
	pip->ttl      = 1;
	pip->daddr    = fl4.daddr;
	pip->saddr    = fl4.saddr;
	pip->protocol = IPPROTO_IGMP;
	pip->tot_len  = 0;	/* filled in later */
	ip_select_ident(pip, &rt->dst, NULL);
	((u8*)&pip[1])[0] = IPOPT_RA;
	((u8*)&pip[1])[1] = 4;
	((u8*)&pip[1])[2] = 0;
	((u8*)&pip[1])[3] = 0;

	skb->transport_header = skb->network_header + sizeof(struct iphdr) + 4;
	skb_put(skb, sizeof(*pig));
	pig = igmpv3_report_hdr(skb);
	pig->type = IGMPV3_HOST_MEMBERSHIP_REPORT;
	pig->resv1 = 0;
	pig->csum = 0;
	pig->resv2 = 0;
	pig->ngrec = 0;
	return skb;
}
