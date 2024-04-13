static inline int __br_ip4_hash(struct net_bridge_mdb_htable *mdb, __be32 ip,
				__u16 vid)
{
	return jhash_2words((__force u32)ip, vid, mdb->secret) & (mdb->max - 1);
}
