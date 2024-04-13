static inline void lsi_mem_write(LSIState *s, dma_addr_t addr,
                                const void *buf, dma_addr_t len)
{
    if (s->dmode & LSI_DMODE_DIOM) {
        address_space_write(&s->pci_io_as, addr, MEMTXATTRS_UNSPECIFIED,
                            buf, len);
    } else {
        pci_dma_write(PCI_DEVICE(s), addr, buf, len);
    }
}
