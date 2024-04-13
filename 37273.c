static __always_inline void slab_free(struct kmem_cache *s,
			struct page *page, void *x, void *addr)
{
	void **object = (void *)x;
	struct kmem_cache_cpu *c;
	unsigned long flags;

	local_irq_save(flags);
	c = get_cpu_slab(s, smp_processor_id());
	debug_check_no_locks_freed(object, c->objsize);
	if (!(s->flags & SLAB_DEBUG_OBJECTS))
		debug_check_no_obj_freed(object, s->objsize);
	if (likely(page == c->page && c->node >= 0)) {
		object[c->offset] = c->freelist;
		c->freelist = object;
		stat(c, FREE_FASTPATH);
	} else
		__slab_free(s, page, x, addr, c->offset);

	local_irq_restore(flags);
}
