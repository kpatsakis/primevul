static int free_debug_processing(struct kmem_cache *s, struct page *page,
						void *object, void *addr)
{
	if (!check_slab(s, page))
		goto fail;

	if (!check_valid_pointer(s, page, object)) {
		slab_err(s, page, "Invalid object pointer 0x%p", object);
		goto fail;
	}

	if (on_freelist(s, page, object)) {
		object_err(s, page, object, "Object already free");
		goto fail;
	}

	if (!check_object(s, page, object, 1))
		return 0;

	if (unlikely(s != page->slab)) {
		if (!PageSlab(page)) {
			slab_err(s, page, "Attempt to free object(0x%p) "
				"outside of slab", object);
		} else if (!page->slab) {
			printk(KERN_ERR
				"SLUB <none>: no slab for object 0x%p.\n",
						object);
			dump_stack();
		} else
			object_err(s, page, object,
					"page slab pointer corrupt.");
		goto fail;
	}

	/* Special debug activities for freeing objects */
	if (!SlabFrozen(page) && !page->freelist)
		remove_full(s, page);
	if (s->flags & SLAB_STORE_USER)
		set_track(s, object, TRACK_FREE, addr);
	trace(s, page, object, 0);
	init_object(s, object, 0);
	return 1;

fail:
	slab_fix(s, "Object at 0x%p not freed", object);
	return 0;
}
