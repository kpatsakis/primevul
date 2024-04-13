static XHCIStreamContext *xhci_alloc_stream_contexts(unsigned count,
                                                     dma_addr_t base)
{
    XHCIStreamContext *stctx;
    unsigned int i;

    stctx = g_new0(XHCIStreamContext, count);
    for (i = 0; i < count; i++) {
        stctx[i].pctx = base + i * 16;
        stctx[i].sct = -1;
    }
    return stctx;
}
