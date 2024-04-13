static enum b43_dmatype dma_mask_to_engine_type(u64 dmamask)
{
	if (dmamask == DMA_BIT_MASK(30))
		return B43_DMA_30BIT;
	if (dmamask == DMA_BIT_MASK(32))
		return B43_DMA_32BIT;
	if (dmamask == DMA_BIT_MASK(64))
		return B43_DMA_64BIT;
	B43_WARN_ON(1);
	return B43_DMA_30BIT;
}
