static void deactivate_slab(struct kmem_cache *s, struct kmem_cache_cpu *c)
{
	struct page *page = c->page;
	int tail = 1;

	if (page->freelist)
		stat(c, DEACTIVATE_REMOTE_FREES);
	/*
	 * Merge cpu freelist into slab freelist. Typically we get here
	 * because both freelists are empty. So this is unlikely
	 * to occur.
	 */
	while (unlikely(c->freelist)) {
		void **object;

		tail = 0;	/* Hot objects. Put the slab first */

		/* Retrieve object from cpu_freelist */
		object = c->freelist;
		c->freelist = c->freelist[c->offset];

		/* And put onto the regular freelist */
		object[c->offset] = page->freelist;
		page->freelist = object;
		page->inuse--;
	}
	c->page = NULL;
	unfreeze_slab(s, page, tail);
}
