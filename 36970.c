static u64 supported_dma_mask(struct b43_wldev *dev)
{
	u32 tmp;
	u16 mmio_base;

	tmp = b43_read32(dev, SSB_TMSHIGH);
	if (tmp & SSB_TMSHIGH_DMA64)
		return DMA_BIT_MASK(64);
	mmio_base = b43_dmacontroller_base(0, 0);
	b43_write32(dev, mmio_base + B43_DMA32_TXCTL, B43_DMA32_TXADDREXT_MASK);
	tmp = b43_read32(dev, mmio_base + B43_DMA32_TXCTL);
	if (tmp & B43_DMA32_TXADDREXT_MASK)
		return DMA_BIT_MASK(32);

	return DMA_BIT_MASK(30);
}
