static void init_kmem_cache_cpu(struct kmem_cache *s,
			struct kmem_cache_cpu *c)
{
	c->page = NULL;
	c->freelist = NULL;
	c->node = 0;
	c->offset = s->offset / sizeof(void *);
	c->objsize = s->objsize;
#ifdef CONFIG_SLUB_STATS
	memset(c->stat, 0, NR_SLUB_STAT_ITEMS * sizeof(unsigned));
#endif
}
