static inline int kmem_cache_close(struct kmem_cache *s)
{
	int node;

	flush_all(s);

	/* Attempt to free all objects */
	free_kmem_cache_cpus(s);
	for_each_node_state(node, N_NORMAL_MEMORY) {
		struct kmem_cache_node *n = get_node(s, node);

		free_partial(s, n);
		if (n->nr_partial || slabs_node(s, node))
			return 1;
	}
	free_kmem_cache_nodes(s);
	return 0;
}
