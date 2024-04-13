static void *kmalloc_large_node(size_t size, gfp_t flags, int node)
{
	struct page *page = alloc_pages_node(node, flags | __GFP_COMP,
						get_order(size));

	if (page)
		return page_address(page);
	else
		return NULL;
}
