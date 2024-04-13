static inline int ping_hashfn(struct net *net, unsigned int num, unsigned int mask)
{
	int res = (num + net_hash_mix(net)) & mask;

	pr_debug("hash(%d) = %d\n", num, res);
	return res;
}
