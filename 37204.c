void *kmem_cache_alloc(struct kmem_cache *s, gfp_t gfpflags)
{
	return slab_alloc(s, gfpflags, -1, __builtin_return_address(0));
}
