static int alloc_ringmemory(struct b43_dmaring *ring)
{
	gfp_t flags = GFP_KERNEL;

	/* The specs call for 4K buffers for 30- and 32-bit DMA with 4K
	 * alignment and 8K buffers for 64-bit DMA with 8K alignment. Testing
	 * has shown that 4K is sufficient for the latter as long as the buffer
	 * does not cross an 8K boundary.
	 *
	 * For unknown reasons - possibly a hardware error - the BCM4311 rev
	 * 02, which uses 64-bit DMA, needs the ring buffer in very low memory,
	 * which accounts for the GFP_DMA flag below.
	 *
	 * The flags here must match the flags in free_ringmemory below!
	 */
	if (ring->type == B43_DMA_64BIT)
		flags |= GFP_DMA;
	ring->descbase = dma_alloc_coherent(ring->dev->dev->dma_dev,
					    B43_DMA_RINGMEMSIZE,
					    &(ring->dmabase), flags);
	if (!ring->descbase) {
		b43err(ring->dev->wl, "DMA ringmemory allocation failed\n");
		return -ENOMEM;
	}
	memset(ring->descbase, 0, B43_DMA_RINGMEMSIZE);

	return 0;
}
