static int s_show(struct seq_file *m, void *p)
{
	unsigned long nr_partials = 0;
	unsigned long nr_slabs = 0;
	unsigned long nr_inuse = 0;
	unsigned long nr_objs = 0;
	unsigned long nr_free = 0;
	struct kmem_cache *s;
	int node;

	s = list_entry(p, struct kmem_cache, list);

	for_each_online_node(node) {
		struct kmem_cache_node *n = get_node(s, node);

		if (!n)
			continue;

		nr_partials += n->nr_partial;
		nr_slabs += atomic_long_read(&n->nr_slabs);
		nr_objs += atomic_long_read(&n->total_objects);
		nr_free += count_partial(n, count_free);
	}

	nr_inuse = nr_objs - nr_free;

	seq_printf(m, "%-17s %6lu %6lu %6u %4u %4d", s->name, nr_inuse,
		   nr_objs, s->size, oo_objects(s->oo),
		   (1 << oo_order(s->oo)));
	seq_printf(m, " : tunables %4u %4u %4u", 0, 0, 0);
	seq_printf(m, " : slabdata %6lu %6lu %6lu", nr_slabs, nr_slabs,
		   0UL);
	seq_putc(m, '\n');
	return 0;
}
