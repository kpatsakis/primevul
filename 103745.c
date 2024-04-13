static TRBCCode xhci_get_port_bandwidth(XHCIState *xhci, uint64_t pctx)
{
    dma_addr_t ctx;
    uint8_t bw_ctx[xhci->numports+1];

    DPRINTF("xhci_get_port_bandwidth()\n");

    ctx = xhci_mask64(pctx);

    DPRINTF("xhci: bandwidth context at "DMA_ADDR_FMT"\n", ctx);

    /* TODO: actually implement real values here */
    bw_ctx[0] = 0;
    memset(&bw_ctx[1], 80, xhci->numports); /* 80% */
    pci_dma_write(PCI_DEVICE(xhci), ctx, bw_ctx, sizeof(bw_ctx));

    return CC_SUCCESS;
}
