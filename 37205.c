void *kmem_cache_alloc_node(struct kmem_cache *s, gfp_t gfpflags, int node)
{
	return slab_alloc(s, gfpflags, node, __builtin_return_address(0));
}
