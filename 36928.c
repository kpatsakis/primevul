static int b43_dma_set_mask(struct b43_wldev *dev, u64 mask)
{
	u64 orig_mask = mask;
	bool fallback = 0;
	int err;

	/* Try to set the DMA mask. If it fails, try falling back to a
	 * lower mask, as we can always also support a lower one. */
	while (1) {
		err = dma_set_mask(dev->dev->dma_dev, mask);
		if (!err) {
			err = dma_set_coherent_mask(dev->dev->dma_dev, mask);
			if (!err)
				break;
		}
		if (mask == DMA_BIT_MASK(64)) {
			mask = DMA_BIT_MASK(32);
			fallback = 1;
			continue;
		}
		if (mask == DMA_BIT_MASK(32)) {
			mask = DMA_BIT_MASK(30);
			fallback = 1;
			continue;
		}
		b43err(dev->wl, "The machine/kernel does not support "
		       "the required %u-bit DMA mask\n",
		       (unsigned int)dma_mask_to_engine_type(orig_mask));
		return -EOPNOTSUPP;
	}
	if (fallback) {
		b43info(dev->wl, "DMA mask fallback from %u-bit to %u-bit\n",
			(unsigned int)dma_mask_to_engine_type(orig_mask),
			(unsigned int)dma_mask_to_engine_type(mask));
	}

	return 0;
}
