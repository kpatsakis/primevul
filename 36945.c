static void free_ringmemory(struct b43_dmaring *ring)
{
	dma_free_coherent(ring->dev->dev->dma_dev, B43_DMA_RINGMEMSIZE,
			  ring->descbase, ring->dmabase);
}
