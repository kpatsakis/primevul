struct net_bridge_mdb_entry *br_mdb_get(struct net_bridge *br,
					struct sk_buff *skb)
{
	struct net_bridge_mdb_htable *mdb = rcu_dereference(br->mdb);
	struct br_ip ip;

	if (br->multicast_disabled)
		return NULL;

	if (BR_INPUT_SKB_CB(skb)->igmp)
		return NULL;

	ip.proto = skb->protocol;

	switch (skb->protocol) {
	case htons(ETH_P_IP):
		ip.u.ip4 = ip_hdr(skb)->daddr;
		break;
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	case htons(ETH_P_IPV6):
		ipv6_addr_copy(&ip.u.ip6, &ipv6_hdr(skb)->daddr);
		break;
#endif
	default:
		return NULL;
	}

	return br_mdb_ip_get(mdb, &ip);
}
