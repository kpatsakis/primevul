static int init_kmem_cache_nodes(struct kmem_cache *s, gfp_t gfpflags)
{
	init_kmem_cache_node(&s->local_node);
	return 1;
}
