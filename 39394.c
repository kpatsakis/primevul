static inline int __br_ip6_hash(struct net_bridge_mdb_htable *mdb,
				const struct in6_addr *ip,
				__u16 vid)
{
	return jhash_2words(ipv6_addr_hash(ip), vid,
			    mdb->secret) & (mdb->max - 1);
}
