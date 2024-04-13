 void ip6_frag_init(struct inet_frag_queue *q, void *a)
 {
 	struct frag_queue *fq = container_of(q, struct frag_queue, q);
	struct ip6_create_arg *arg = a;

	fq->id = arg->id;
	fq->user = arg->user;
	ipv6_addr_copy(&fq->saddr, arg->src);
	ipv6_addr_copy(&fq->daddr, arg->dst);
}
