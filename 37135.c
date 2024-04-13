void *__kmalloc_track_caller(size_t size, gfp_t gfpflags, void *caller)
{
	struct kmem_cache *s;

	if (unlikely(size > PAGE_SIZE))
		return kmalloc_large(size, gfpflags);

	s = get_slab(size, gfpflags);

	if (unlikely(ZERO_OR_NULL_PTR(s)))
		return s;

	return slab_alloc(s, gfpflags, -1, caller);
}
