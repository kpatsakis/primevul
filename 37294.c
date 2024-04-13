static void sysfs_add_func(struct work_struct *w)
{
	struct kmem_cache *s;

	down_write(&slub_lock);
	list_for_each_entry(s, &slab_caches, list) {
		if (s->flags & __SYSFS_ADD_DEFERRED) {
			s->flags &= ~__SYSFS_ADD_DEFERRED;
			sysfs_slab_add(s);
		}
	}
	up_write(&slub_lock);
}
