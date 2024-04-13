static inline int oo_order(struct kmem_cache_order_objects x)
{
	return x.x >> 16;
}
