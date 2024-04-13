void kfree(const void *x)
{
	struct page *page;
	void *object = (void *)x;

	if (unlikely(ZERO_OR_NULL_PTR(x)))
		return;

	page = virt_to_head_page(x);
	if (unlikely(!PageSlab(page))) {
		put_page(page);
		return;
	}
	slab_free(page->slab, page, object, __builtin_return_address(0));
}
