static struct kmem_cache_node *early_kmem_cache_node_alloc(gfp_t gfpflags,
							   int node)
{
	struct page *page;
	struct kmem_cache_node *n;
	unsigned long flags;

	BUG_ON(kmalloc_caches->size < sizeof(struct kmem_cache_node));

	page = new_slab(kmalloc_caches, gfpflags, node);

	BUG_ON(!page);
	if (page_to_nid(page) != node) {
		printk(KERN_ERR "SLUB: Unable to allocate memory from "
				"node %d\n", node);
		printk(KERN_ERR "SLUB: Allocating a useless per node structure "
				"in order to be able to continue\n");
	}

	n = page->freelist;
	BUG_ON(!n);
	page->freelist = get_freepointer(kmalloc_caches, n);
	page->inuse++;
	kmalloc_caches->node[node] = n;
#ifdef CONFIG_SLUB_DEBUG
	init_object(kmalloc_caches, n, 1);
	init_tracking(kmalloc_caches, n);
#endif
	init_kmem_cache_node(n);
	inc_slabs_node(kmalloc_caches, node, page->objects);

	/*
	 * lockdep requires consistent irq usage for each lock
	 * so even though there cannot be a race this early in
	 * the boot sequence, we still disable irqs.
	 */
	local_irq_save(flags);
	add_partial(n, page, 0);
	local_irq_restore(flags);
	return n;
}
