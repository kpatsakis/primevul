static inline void sysfs_slab_remove(struct kmem_cache *s)
{
	kfree(s);
}
