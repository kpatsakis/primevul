static inline dma_addr_t desc_to_dma(struct netdev_desc *desc)
{
	return le64_to_cpu(desc->fraginfo) & DMA_BIT_MASK(48);
}
