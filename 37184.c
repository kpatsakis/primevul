static inline struct kmem_cache_cpu *get_cpu_slab(struct kmem_cache *s, int cpu)
{
#ifdef CONFIG_SMP
	return s->cpu_slab[cpu];
#else
	return &s->cpu_slab;
#endif
}
