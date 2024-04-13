static void *__slab_alloc(struct kmem_cache *s,
		gfp_t gfpflags, int node, void *addr, struct kmem_cache_cpu *c)
{
	void **object;
	struct page *new;

	/* We handle __GFP_ZERO in the caller */
	gfpflags &= ~__GFP_ZERO;

	if (!c->page)
		goto new_slab;

	slab_lock(c->page);
	if (unlikely(!node_match(c, node)))
		goto another_slab;

	stat(c, ALLOC_REFILL);

load_freelist:
	object = c->page->freelist;
	if (unlikely(!object))
		goto another_slab;
	if (unlikely(SlabDebug(c->page)))
		goto debug;

	c->freelist = object[c->offset];
	c->page->inuse = c->page->objects;
	c->page->freelist = NULL;
	c->node = page_to_nid(c->page);
unlock_out:
	slab_unlock(c->page);
	stat(c, ALLOC_SLOWPATH);
	return object;

another_slab:
	deactivate_slab(s, c);

new_slab:
	new = get_partial(s, gfpflags, node);
	if (new) {
		c->page = new;
		stat(c, ALLOC_FROM_PARTIAL);
		goto load_freelist;
	}

	if (gfpflags & __GFP_WAIT)
		local_irq_enable();

	new = new_slab(s, gfpflags, node);

	if (gfpflags & __GFP_WAIT)
		local_irq_disable();

	if (new) {
		c = get_cpu_slab(s, smp_processor_id());
		stat(c, ALLOC_SLAB);
		if (c->page)
			flush_slab(s, c);
		slab_lock(new);
		SetSlabFrozen(new);
		c->page = new;
		goto load_freelist;
	}
	return NULL;
debug:
	if (!alloc_debug_processing(s, c->page, object, addr))
		goto another_slab;

	c->page->inuse++;
	c->page->freelist = object[c->offset];
	c->node = -1;
	goto unlock_out;
}
