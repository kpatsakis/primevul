static inline void ClearSlabFrozen(struct page *page)
{
	page->flags &= ~FROZEN;
}
