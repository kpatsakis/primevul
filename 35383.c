int ip6_frag_nqueues(struct net *net)
{
	return net->ipv6.frags.nqueues;
}
