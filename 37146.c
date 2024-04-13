static inline int alloc_kmem_cache_cpus(struct kmem_cache *s, gfp_t flags)
{
	init_kmem_cache_cpu(s, &s->cpu_slab);
	return 1;
}
