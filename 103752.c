static inline dma_addr_t xhci_mask64(uint64_t addr)
{
    if (sizeof(dma_addr_t) == 4) {
        return addr & 0xffffffff;
    } else {
        return addr;
    }
}
