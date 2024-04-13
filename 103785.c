static void xhci_via_challenge(XHCIState *xhci, uint64_t addr)
{
    PCIDevice *pci_dev = PCI_DEVICE(xhci);
    uint32_t buf[8];
    uint32_t obuf[8];
    dma_addr_t paddr = xhci_mask64(addr);

    pci_dma_read(pci_dev, paddr, &buf, 32);

    memcpy(obuf, buf, sizeof(obuf));

    if ((buf[0] & 0xff) == 2) {
        obuf[0] = 0x49932000 + 0x54dc200 * buf[2] + 0x7429b578 * buf[3];
        obuf[0] |=  (buf[2] * buf[3]) & 0xff;
        obuf[1] = 0x0132bb37 + 0xe89 * buf[2] + 0xf09 * buf[3];
        obuf[2] = 0x0066c2e9 + 0x2091 * buf[2] + 0x19bd * buf[3];
        obuf[3] = 0xd5281342 + 0x2cc9691 * buf[2] + 0x2367662 * buf[3];
        obuf[4] = 0x0123c75c + 0x1595 * buf[2] + 0x19ec * buf[3];
        obuf[5] = 0x00f695de + 0x26fd * buf[2] + 0x3e9 * buf[3];
        obuf[6] = obuf[2] ^ obuf[3] ^ 0x29472956;
        obuf[7] = obuf[2] ^ obuf[3] ^ 0x65866593;
    }

    pci_dma_write(pci_dev, paddr, &obuf, 32);
}
