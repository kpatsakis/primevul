void *__kmalloc_node_track_caller(size_t size, gfp_t gfpflags,
					int node, void *caller)
{
	struct kmem_cache *s;

	if (unlikely(size > PAGE_SIZE))
		return kmalloc_large_node(size, gfpflags, node);

	s = get_slab(size, gfpflags);

	if (unlikely(ZERO_OR_NULL_PTR(s)))
		return s;

	return slab_alloc(s, gfpflags, node, caller);
}
