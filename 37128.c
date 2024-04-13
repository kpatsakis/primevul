static inline void SetSlabFrozen(struct page *page)
{
	page->flags |= FROZEN;
}
