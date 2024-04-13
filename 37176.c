static inline void flush_slab(struct kmem_cache *s, struct kmem_cache_cpu *c)
{
	stat(c, CPUSLAB_FLUSH);
	slab_lock(c->page);
	deactivate_slab(s, c);
}
