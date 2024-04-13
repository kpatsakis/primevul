static ssize_t remote_node_defrag_ratio_store(struct kmem_cache *s,
				const char *buf, size_t length)
{
	int n = simple_strtoul(buf, NULL, 10);

	if (n < 100)
		s->remote_node_defrag_ratio = n * 10;
	return length;
}
