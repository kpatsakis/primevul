static void validate_slab_slab(struct kmem_cache *s, struct page *page,
						unsigned long *map)
{
	if (slab_trylock(page)) {
		validate_slab(s, page, map);
		slab_unlock(page);
	} else
		printk(KERN_INFO "SLUB %s: Skipped busy slab 0x%p\n",
			s->name, page);

	if (s->flags & DEBUG_DEFAULT_FLAGS) {
		if (!SlabDebug(page))
			printk(KERN_ERR "SLUB %s: SlabDebug not set "
				"on slab 0x%p\n", s->name, page);
	} else {
		if (SlabDebug(page))
			printk(KERN_ERR "SLUB %s: SlabDebug set on "
				"slab 0x%p\n", s->name, page);
	}
}
