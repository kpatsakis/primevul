void *__kmalloc(size_t size, gfp_t flags)
{
	struct kmem_cache *s;

	if (unlikely(size > PAGE_SIZE))
		return kmalloc_large(size, flags);

	s = get_slab(size, flags);

	if (unlikely(ZERO_OR_NULL_PTR(s)))
		return s;

	return slab_alloc(s, flags, -1, __builtin_return_address(0));
}
