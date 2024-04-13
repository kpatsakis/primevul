static noinline struct kmem_cache *dma_kmalloc_cache(int index, gfp_t flags)
{
	struct kmem_cache *s;
	char *text;
	size_t realsize;

	s = kmalloc_caches_dma[index];
	if (s)
		return s;

	/* Dynamically create dma cache */
	if (flags & __GFP_WAIT)
		down_write(&slub_lock);
	else {
		if (!down_write_trylock(&slub_lock))
			goto out;
	}

	if (kmalloc_caches_dma[index])
		goto unlock_out;

	realsize = kmalloc_caches[index].objsize;
	text = kasprintf(flags & ~SLUB_DMA, "kmalloc_dma-%d",
			 (unsigned int)realsize);
	s = kmalloc(kmem_size, flags & ~SLUB_DMA);

	if (!s || !text || !kmem_cache_open(s, flags, text,
			realsize, ARCH_KMALLOC_MINALIGN,
			SLAB_CACHE_DMA|__SYSFS_ADD_DEFERRED, NULL)) {
		kfree(s);
		kfree(text);
		goto unlock_out;
	}

	list_add(&s->list, &slab_caches);
	kmalloc_caches_dma[index] = s;

	schedule_work(&sysfs_add_work);

unlock_out:
	up_write(&slub_lock);
out:
	return kmalloc_caches_dma[index];
}
