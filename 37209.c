static inline unsigned long kmem_cache_flags(unsigned long objsize,
	unsigned long flags, const char *name,
	void (*ctor)(struct kmem_cache *, void *))
{
	return flags;
}
