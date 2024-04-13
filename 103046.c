static inline void lsi_mem_read(LSIState *s, dma_addr_t addr,
                               void *buf, dma_addr_t len)
{
    if (s->dmode & LSI_DMODE_SIOM) {
        address_space_read(&s->pci_io_as, addr, MEMTXATTRS_UNSPECIFIED,
                           buf, len);
    } else {
        pci_dma_read(PCI_DEVICE(s), addr, buf, len);
    }
}
