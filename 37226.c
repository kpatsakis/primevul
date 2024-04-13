static ssize_t order_store(struct kmem_cache *s,
				const char *buf, size_t length)
{
	int order = simple_strtoul(buf, NULL, 10);

	if (order > slub_max_order || order < slub_min_order)
		return -EINVAL;

	calculate_sizes(s, order);
	return length;
}
