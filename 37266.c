static __always_inline void *slab_alloc(struct kmem_cache *s,
		gfp_t gfpflags, int node, void *addr)
{
	void **object;
	struct kmem_cache_cpu *c;
	unsigned long flags;

	local_irq_save(flags);
	c = get_cpu_slab(s, smp_processor_id());
	if (unlikely(!c->freelist || !node_match(c, node)))

		object = __slab_alloc(s, gfpflags, node, addr, c);

	else {
		object = c->freelist;
		c->freelist = object[c->offset];
		stat(c, ALLOC_FASTPATH);
	}
	local_irq_restore(flags);

	if (unlikely((gfpflags & __GFP_ZERO) && object))
		memset(object, 0, c->objsize);

	return object;
}
