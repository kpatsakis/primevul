check_pages_isolated_cb(unsigned long start_pfn, unsigned long nr_pages,
			void *data)
{
	int ret;
	long offlined = *(long *)data;
	ret = test_pages_isolated(start_pfn, start_pfn + nr_pages);
	offlined = nr_pages;
	if (!ret)
		*(long *)data += offlined;
	return ret;
}
