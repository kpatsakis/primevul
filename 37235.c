static void process_slab(struct loc_track *t, struct kmem_cache *s,
		struct page *page, enum track_item alloc)
{
	void *addr = page_address(page);
	DECLARE_BITMAP(map, page->objects);
	void *p;

	bitmap_zero(map, page->objects);
	for_each_free_object(p, s, page->freelist)
		set_bit(slab_index(p, s, addr), map);

	for_each_object(p, s, addr, page->objects)
		if (!test_bit(slab_index(p, s, addr), map))
			add_location(t, s, get_track(s, p, alloc));
}
