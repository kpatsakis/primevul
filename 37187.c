static struct page *get_object_page(const void *x)
{
	struct page *page = virt_to_head_page(x);

	if (!PageSlab(page))
		return NULL;

	return page;
}
