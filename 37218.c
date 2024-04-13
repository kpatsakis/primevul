static inline int lock_and_freeze_slab(struct kmem_cache_node *n, struct page *page)
{
	if (slab_trylock(page)) {
		list_del(&page->lru);
		n->nr_partial--;
		SetSlabFrozen(page);
		return 1;
	}
	return 0;
}
