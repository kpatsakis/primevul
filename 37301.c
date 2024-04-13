static void trace(struct kmem_cache *s, struct page *page, void *object, int alloc)
{
	if (s->flags & SLAB_TRACE) {
		printk(KERN_INFO "TRACE %s %s 0x%p inuse=%d fp=0x%p\n",
			s->name,
			alloc ? "alloc" : "free",
			object, page->inuse,
			page->freelist);

		if (!alloc)
			print_section("Object", (void *)object, s->objsize);

		dump_stack();
	}
}
