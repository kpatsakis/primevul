static void op64_poke_tx(struct b43_dmaring *ring, int slot)
{
	b43_dma_write(ring, B43_DMA64_TXINDEX,
		      (u32) (slot * sizeof(struct b43_dmadesc64)));
}
