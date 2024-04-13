static void remove_full(struct kmem_cache *s, struct page *page)
{
	struct kmem_cache_node *n;

	if (!(s->flags & SLAB_STORE_USER))
		return;

	n = get_node(s, page_to_nid(page));

	spin_lock(&n->list_lock);
	list_del(&page->lru);
	spin_unlock(&n->list_lock);
}
