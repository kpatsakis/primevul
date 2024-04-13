void *__kmalloc_node(size_t size, gfp_t flags, int node)
{
	struct kmem_cache *s;

	if (unlikely(size > PAGE_SIZE))
		return kmalloc_large_node(size, flags, node);

	s = get_slab(size, flags);

	if (unlikely(ZERO_OR_NULL_PTR(s)))
		return s;

	return slab_alloc(s, flags, node, __builtin_return_address(0));
}
