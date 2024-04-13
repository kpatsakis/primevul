static int slab_mem_going_offline_callback(void *arg)
{
	struct kmem_cache *s;

	down_read(&slub_lock);
	list_for_each_entry(s, &slab_caches, list)
		kmem_cache_shrink(s);
	up_read(&slub_lock);

	return 0;
}
