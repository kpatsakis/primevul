static inline uint32_t read_dword(LSIState *s, uint32_t addr)
{
    uint32_t buf;

    pci_dma_read(PCI_DEVICE(s), addr, &buf, 4);
    return cpu_to_le32(buf);
}
