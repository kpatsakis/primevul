static inline struct kmem_cache_order_objects oo_make(int order,
						unsigned long size)
{
	struct kmem_cache_order_objects x = {
		(order << 16) + (PAGE_SIZE << order) / size
	};

	return x;
}
