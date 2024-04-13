static inline int SlabDebug(struct page *page)
{
	return page->flags & SLABDEBUG;
}
