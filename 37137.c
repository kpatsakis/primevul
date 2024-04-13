static void __slab_free(struct kmem_cache *s, struct page *page,
				void *x, void *addr, unsigned int offset)
{
	void *prior;
	void **object = (void *)x;
	struct kmem_cache_cpu *c;

	c = get_cpu_slab(s, raw_smp_processor_id());
	stat(c, FREE_SLOWPATH);
	slab_lock(page);

	if (unlikely(SlabDebug(page)))
		goto debug;

checks_ok:
	prior = object[offset] = page->freelist;
	page->freelist = object;
	page->inuse--;

	if (unlikely(SlabFrozen(page))) {
		stat(c, FREE_FROZEN);
		goto out_unlock;
	}

	if (unlikely(!page->inuse))
		goto slab_empty;

	/*
	 * Objects left in the slab. If it was not on the partial list before
	 * then add it.
	 */
	if (unlikely(!prior)) {
		add_partial(get_node(s, page_to_nid(page)), page, 1);
		stat(c, FREE_ADD_PARTIAL);
	}

out_unlock:
	slab_unlock(page);
	return;

slab_empty:
	if (prior) {
		/*
		 * Slab still on the partial list.
		 */
		remove_partial(s, page);
		stat(c, FREE_REMOVE_PARTIAL);
	}
	slab_unlock(page);
	stat(c, FREE_SLAB);
	discard_slab(s, page);
	return;

debug:
	if (!free_debug_processing(s, page, x, addr))
		goto out_unlock;
	goto checks_ok;
}
