static void flush_all(struct kmem_cache *s)
{
#ifdef CONFIG_SMP
	on_each_cpu(flush_cpu_slab, s, 1, 1);
#else
	unsigned long flags;

	local_irq_save(flags);
	flush_cpu_slab(s);
	local_irq_restore(flags);
#endif
}
