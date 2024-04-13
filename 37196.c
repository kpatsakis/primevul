static void init_alloc_cpu_cpu(int cpu)
{
	int i;

	if (cpu_isset(cpu, kmem_cach_cpu_free_init_once))
		return;

	for (i = NR_KMEM_CACHE_CPU - 1; i >= 0; i--)
		free_kmem_cache_cpu(&per_cpu(kmem_cache_cpu, cpu)[i], cpu);

	cpu_set(cpu, kmem_cach_cpu_free_init_once);
}
