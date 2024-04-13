static void xhci_init_epctx(XHCIEPContext *epctx,
                            dma_addr_t pctx, uint32_t *ctx)
{
    dma_addr_t dequeue;

    dequeue = xhci_addr64(ctx[2] & ~0xf, ctx[3]);

    epctx->type = (ctx[1] >> EP_TYPE_SHIFT) & EP_TYPE_MASK;
    epctx->pctx = pctx;
    epctx->max_psize = ctx[1]>>16;
    epctx->max_psize *= 1+((ctx[1]>>8)&0xff);
    epctx->max_pstreams = (ctx[0] >> 10) & epctx->xhci->max_pstreams_mask;
    epctx->lsa = (ctx[0] >> 15) & 1;
    if (epctx->max_pstreams) {
        xhci_alloc_streams(epctx, dequeue);
    } else {
        xhci_ring_init(epctx->xhci, &epctx->ring, dequeue);
        epctx->ring.ccs = ctx[2] & 1;
    }

    epctx->interval = 1 << ((ctx[0] >> 16) & 0xff);
}
