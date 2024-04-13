fq_find(struct net *net, __be32 id, struct in6_addr *src, struct in6_addr *dst)
{
	struct inet_frag_queue *q;
	struct ip6_create_arg arg;
	unsigned int hash;

	arg.id = id;
	arg.user = IP6_DEFRAG_LOCAL_DELIVER;
	arg.src = src;
	arg.dst = dst;

	read_lock(&ip6_frags.lock);
	hash = inet6_hash_frag(id, src, dst, ip6_frags.rnd);

	q = inet_frag_find(&net->ipv6.frags, &ip6_frags, &arg, hash);
	if (q == NULL)
		return NULL;

	return container_of(q, struct frag_queue, q);
}
