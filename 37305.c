static void unfreeze_slab(struct kmem_cache *s, struct page *page, int tail)
{
	struct kmem_cache_node *n = get_node(s, page_to_nid(page));
	struct kmem_cache_cpu *c = get_cpu_slab(s, smp_processor_id());

	ClearSlabFrozen(page);
	if (page->inuse) {

		if (page->freelist) {
			add_partial(n, page, tail);
			stat(c, tail ? DEACTIVATE_TO_TAIL : DEACTIVATE_TO_HEAD);
		} else {
			stat(c, DEACTIVATE_FULL);
			if (SlabDebug(page) && (s->flags & SLAB_STORE_USER))
				add_full(n, page);
		}
		slab_unlock(page);
	} else {
		stat(c, DEACTIVATE_EMPTY);
		if (n->nr_partial < MIN_PARTIAL) {
			/*
			 * Adding an empty slab to the partial slabs in order
			 * to avoid page allocator overhead. This slab needs
			 * to come after the other slabs with objects in
			 * so that the others get filled first. That way the
			 * size of the partial list stays small.
			 *
			 * kmem_cache_shrink can reclaim any empty slabs from the
			 * partial list.
			 */
			add_partial(n, page, 1);
			slab_unlock(page);
		} else {
			slab_unlock(page);
			stat(get_cpu_slab(s, raw_smp_processor_id()), FREE_SLAB);
			discard_slab(s, page);
		}
	}
}
