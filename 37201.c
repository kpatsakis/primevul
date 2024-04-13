static void init_tracking(struct kmem_cache *s, void *object)
{
	if (!(s->flags & SLAB_STORE_USER))
		return;

	set_track(s, object, TRACK_FREE, NULL);
	set_track(s, object, TRACK_ALLOC, NULL);
}
