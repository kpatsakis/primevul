static void xhci_alloc_streams(XHCIEPContext *epctx, dma_addr_t base)
{
    assert(epctx->pstreams == NULL);
    epctx->nr_pstreams = 2 << epctx->max_pstreams;
    epctx->pstreams = xhci_alloc_stream_contexts(epctx->nr_pstreams, base);
}
