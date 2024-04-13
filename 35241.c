static int __remove_section(struct zone *zone, struct mem_section *ms)
{
	/*
	 * XXX: Freeing memmap with vmemmap is not implement yet.
	 *      This should be removed later.
	 */
	return -EBUSY;
}
