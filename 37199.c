static int init_kmem_cache_nodes(struct kmem_cache *s, gfp_t gfpflags)
{
	int node;
	int local_node;

	if (slab_state >= UP)
		local_node = page_to_nid(virt_to_page(s));
	else
		local_node = 0;

	for_each_node_state(node, N_NORMAL_MEMORY) {
		struct kmem_cache_node *n;

		if (local_node == node)
			n = &s->local_node;
		else {
			if (slab_state == DOWN) {
				n = early_kmem_cache_node_alloc(gfpflags,
								node);
				continue;
			}
			n = kmem_cache_alloc_node(kmalloc_caches,
							gfpflags, node);

			if (!n) {
				free_kmem_cache_nodes(s);
				return 0;
			}

		}
		s->node[node] = n;
		init_kmem_cache_node(n);
	}
	return 1;
}
