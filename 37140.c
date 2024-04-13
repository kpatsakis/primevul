static void add_partial(struct kmem_cache_node *n,
				struct page *page, int tail)
{
	spin_lock(&n->list_lock);
	n->nr_partial++;
	if (tail)
		list_add_tail(&page->lru, &n->partial);
	else
		list_add(&page->lru, &n->partial);
	spin_unlock(&n->list_lock);
}
