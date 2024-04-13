static void map_page(AddressSpace *as, uint8_t **ptr, uint64_t addr,
                     uint32_t wanted)
{
    hwaddr len = wanted;

    if (*ptr) {
        dma_memory_unmap(as, *ptr, len, DMA_DIRECTION_FROM_DEVICE, len);
    }

    *ptr = dma_memory_map(as, addr, &len, DMA_DIRECTION_FROM_DEVICE);
    if (len < wanted) {
        dma_memory_unmap(as, *ptr, len, DMA_DIRECTION_FROM_DEVICE, len);
        *ptr = NULL;
    }
}
