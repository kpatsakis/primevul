static void get_page_bootmem(unsigned long info,  struct page *page,
			     unsigned long type)
{
	page->lru.next = (struct list_head *) type;
	SetPagePrivate(page);
	set_page_private(page, info);
	atomic_inc(&page->_count);
}
