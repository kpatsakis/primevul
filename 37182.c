static void free_partial(struct kmem_cache *s, struct kmem_cache_node *n)
{
	unsigned long flags;
	struct page *page, *h;

	spin_lock_irqsave(&n->list_lock, flags);
	list_for_each_entry_safe(page, h, &n->partial, lru) {
		if (!page->inuse) {
			list_del(&page->lru);
			discard_slab(s, page);
			n->nr_partial--;
		} else {
			list_slab_objects(s, page,
				"Objects remaining on kmem_cache_close()");
		}
	}
	spin_unlock_irqrestore(&n->list_lock, flags);
}
