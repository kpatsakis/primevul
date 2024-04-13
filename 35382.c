int ip6_frag_mem(struct net *net)
{
	return atomic_read(&net->ipv6.frags.mem);
}
