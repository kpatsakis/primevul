static void free_kmem_cache_cpus(struct kmem_cache *s)
{
	int cpu;

	for_each_online_cpu(cpu) {
		struct kmem_cache_cpu *c = get_cpu_slab(s, cpu);

		if (c) {
			s->cpu_slab[cpu] = NULL;
			free_kmem_cache_cpu(c, cpu);
		}
	}
}
