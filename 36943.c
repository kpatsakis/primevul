static int dmacontroller_setup(struct b43_dmaring *ring)
{
	int err = 0;
	u32 value;
	u32 addrext;
	u32 trans = ssb_dma_translation(ring->dev->dev);

	if (ring->tx) {
		if (ring->type == B43_DMA_64BIT) {
			u64 ringbase = (u64) (ring->dmabase);

			addrext = ((ringbase >> 32) & SSB_DMA_TRANSLATION_MASK)
			    >> SSB_DMA_TRANSLATION_SHIFT;
			value = B43_DMA64_TXENABLE;
			value |= (addrext << B43_DMA64_TXADDREXT_SHIFT)
			    & B43_DMA64_TXADDREXT_MASK;
			b43_dma_write(ring, B43_DMA64_TXCTL, value);
			b43_dma_write(ring, B43_DMA64_TXRINGLO,
				      (ringbase & 0xFFFFFFFF));
			b43_dma_write(ring, B43_DMA64_TXRINGHI,
				      ((ringbase >> 32) &
				       ~SSB_DMA_TRANSLATION_MASK)
				      | (trans << 1));
		} else {
			u32 ringbase = (u32) (ring->dmabase);

			addrext = (ringbase & SSB_DMA_TRANSLATION_MASK)
			    >> SSB_DMA_TRANSLATION_SHIFT;
			value = B43_DMA32_TXENABLE;
			value |= (addrext << B43_DMA32_TXADDREXT_SHIFT)
			    & B43_DMA32_TXADDREXT_MASK;
			b43_dma_write(ring, B43_DMA32_TXCTL, value);
			b43_dma_write(ring, B43_DMA32_TXRING,
				      (ringbase & ~SSB_DMA_TRANSLATION_MASK)
				      | trans);
		}
	} else {
		err = alloc_initial_descbuffers(ring);
		if (err)
			goto out;
		if (ring->type == B43_DMA_64BIT) {
			u64 ringbase = (u64) (ring->dmabase);

			addrext = ((ringbase >> 32) & SSB_DMA_TRANSLATION_MASK)
			    >> SSB_DMA_TRANSLATION_SHIFT;
			value = (ring->frameoffset << B43_DMA64_RXFROFF_SHIFT);
			value |= B43_DMA64_RXENABLE;
			value |= (addrext << B43_DMA64_RXADDREXT_SHIFT)
			    & B43_DMA64_RXADDREXT_MASK;
			b43_dma_write(ring, B43_DMA64_RXCTL, value);
			b43_dma_write(ring, B43_DMA64_RXRINGLO,
				      (ringbase & 0xFFFFFFFF));
			b43_dma_write(ring, B43_DMA64_RXRINGHI,
				      ((ringbase >> 32) &
				       ~SSB_DMA_TRANSLATION_MASK)
				      | (trans << 1));
			b43_dma_write(ring, B43_DMA64_RXINDEX, ring->nr_slots *
				      sizeof(struct b43_dmadesc64));
		} else {
			u32 ringbase = (u32) (ring->dmabase);

			addrext = (ringbase & SSB_DMA_TRANSLATION_MASK)
			    >> SSB_DMA_TRANSLATION_SHIFT;
			value = (ring->frameoffset << B43_DMA32_RXFROFF_SHIFT);
			value |= B43_DMA32_RXENABLE;
			value |= (addrext << B43_DMA32_RXADDREXT_SHIFT)
			    & B43_DMA32_RXADDREXT_MASK;
			b43_dma_write(ring, B43_DMA32_RXCTL, value);
			b43_dma_write(ring, B43_DMA32_RXRING,
				      (ringbase & ~SSB_DMA_TRANSLATION_MASK)
				      | trans);
			b43_dma_write(ring, B43_DMA32_RXINDEX, ring->nr_slots *
				      sizeof(struct b43_dmadesc32));
		}
	}

out:
	return err;
}
