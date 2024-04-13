static void xhci_ring_init(XHCIState *xhci, XHCIRing *ring,
                           dma_addr_t base)
{
    ring->dequeue = base;
    ring->ccs = 1;
}
