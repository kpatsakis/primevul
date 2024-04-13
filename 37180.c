static void free_kmem_cache_cpu(struct kmem_cache_cpu *c, int cpu)
{
	if (c < per_cpu(kmem_cache_cpu, cpu) ||
			c > per_cpu(kmem_cache_cpu, cpu) + NR_KMEM_CACHE_CPU) {
		kfree(c);
		return;
	}
	c->freelist = (void *)per_cpu(kmem_cache_cpu_free, cpu);
	per_cpu(kmem_cache_cpu_free, cpu) = c;
}
