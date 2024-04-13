static inline void SetSlabDebug(struct page *page)
{
	page->flags |= SLABDEBUG;
}
