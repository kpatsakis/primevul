void __init kmem_cache_init(void)
{
	int i;
	int caches = 0;

	init_alloc_cpu();

#ifdef CONFIG_NUMA
	/*
	 * Must first have the slab cache available for the allocations of the
	 * struct kmem_cache_node's. There is special bootstrap code in
	 * kmem_cache_open for slab_state == DOWN.
	 */
	create_kmalloc_cache(&kmalloc_caches[0], "kmem_cache_node",
		sizeof(struct kmem_cache_node), GFP_KERNEL);
	kmalloc_caches[0].refcount = -1;
	caches++;

	hotplug_memory_notifier(slab_memory_callback, SLAB_CALLBACK_PRI);
#endif

	/* Able to allocate the per node structures */
	slab_state = PARTIAL;

	/* Caches that are not of the two-to-the-power-of size */
	if (KMALLOC_MIN_SIZE <= 64) {
		create_kmalloc_cache(&kmalloc_caches[1],
				"kmalloc-96", 96, GFP_KERNEL);
		caches++;
	}
	if (KMALLOC_MIN_SIZE <= 128) {
		create_kmalloc_cache(&kmalloc_caches[2],
				"kmalloc-192", 192, GFP_KERNEL);
		caches++;
	}

	for (i = KMALLOC_SHIFT_LOW; i <= PAGE_SHIFT; i++) {
		create_kmalloc_cache(&kmalloc_caches[i],
			"kmalloc", 1 << i, GFP_KERNEL);
		caches++;
	}


	/*
	 * Patch up the size_index table if we have strange large alignment
	 * requirements for the kmalloc array. This is only the case for
	 * MIPS it seems. The standard arches will not generate any code here.
	 *
	 * Largest permitted alignment is 256 bytes due to the way we
	 * handle the index determination for the smaller caches.
	 *
	 * Make sure that nothing crazy happens if someone starts tinkering
	 * around with ARCH_KMALLOC_MINALIGN
	 */
	BUILD_BUG_ON(KMALLOC_MIN_SIZE > 256 ||
		(KMALLOC_MIN_SIZE & (KMALLOC_MIN_SIZE - 1)));

	for (i = 8; i < KMALLOC_MIN_SIZE; i += 8)
		size_index[(i - 1) / 8] = KMALLOC_SHIFT_LOW;

	slab_state = UP;

	/* Provide the correct kmalloc names now that the caches are up */
	for (i = KMALLOC_SHIFT_LOW; i <= PAGE_SHIFT; i++)
		kmalloc_caches[i]. name =
			kasprintf(GFP_KERNEL, "kmalloc-%d", 1 << i);

#ifdef CONFIG_SMP
	register_cpu_notifier(&slab_notifier);
	kmem_size = offsetof(struct kmem_cache, cpu_slab) +
				nr_cpu_ids * sizeof(struct kmem_cache_cpu *);
#else
	kmem_size = sizeof(struct kmem_cache);
#endif

	printk(KERN_INFO
		"SLUB: Genslabs=%d, HWalign=%d, Order=%d-%d, MinObjects=%d,"
		" CPUs=%d, Nodes=%d\n",
		caches, cache_line_size(),
		slub_min_order, slub_max_order, slub_min_objects,
		nr_cpu_ids, nr_node_ids);
}
