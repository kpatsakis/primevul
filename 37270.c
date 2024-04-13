static int __cpuinit slab_cpuup_callback(struct notifier_block *nfb,
		unsigned long action, void *hcpu)
{
	long cpu = (long)hcpu;
	struct kmem_cache *s;
	unsigned long flags;

	switch (action) {
	case CPU_UP_PREPARE:
	case CPU_UP_PREPARE_FROZEN:
		init_alloc_cpu_cpu(cpu);
		down_read(&slub_lock);
		list_for_each_entry(s, &slab_caches, list)
			s->cpu_slab[cpu] = alloc_kmem_cache_cpu(s, cpu,
							GFP_KERNEL);
		up_read(&slub_lock);
		break;

	case CPU_UP_CANCELED:
	case CPU_UP_CANCELED_FROZEN:
	case CPU_DEAD:
	case CPU_DEAD_FROZEN:
		down_read(&slub_lock);
		list_for_each_entry(s, &slab_caches, list) {
			struct kmem_cache_cpu *c = get_cpu_slab(s, cpu);

			local_irq_save(flags);
			__flush_cpu_slab(s, cpu);
			local_irq_restore(flags);
			free_kmem_cache_cpu(c, cpu);
			s->cpu_slab[cpu] = NULL;
		}
		up_read(&slub_lock);
		break;
	default:
		break;
	}
	return NOTIFY_OK;
}
