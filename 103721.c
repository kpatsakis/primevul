static inline void xhci_dma_read_u32s(XHCIState *xhci, dma_addr_t addr,
                                      uint32_t *buf, size_t len)
{
    int i;

    assert((len % sizeof(uint32_t)) == 0);

    pci_dma_read(PCI_DEVICE(xhci), addr, buf, len);

    for (i = 0; i < (len / sizeof(uint32_t)); i++) {
        buf[i] = le32_to_cpu(buf[i]);
    }
}
