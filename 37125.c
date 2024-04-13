static inline void ClearSlabDebug(struct page *page)
{
	page->flags &= ~SLABDEBUG;
}
