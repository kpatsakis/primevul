static inline int node_match(struct kmem_cache_cpu *c, int node)
{
#ifdef CONFIG_NUMA
	if (node != -1 && c->node != node)
		return 0;
#endif
	return 1;
}
