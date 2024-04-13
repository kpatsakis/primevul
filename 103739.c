static XHCIStreamContext *xhci_find_stream(XHCIEPContext *epctx,
                                           unsigned int streamid,
                                           uint32_t *cc_error)
{
    XHCIStreamContext *sctx;
    dma_addr_t base;
    uint32_t ctx[2], sct;

    assert(streamid != 0);
    if (epctx->lsa) {
        if (streamid >= epctx->nr_pstreams) {
            *cc_error = CC_INVALID_STREAM_ID_ERROR;
            return NULL;
        }
        sctx = epctx->pstreams + streamid;
    } else {
        FIXME("secondary streams not implemented yet");
    }

    if (sctx->sct == -1) {
        xhci_dma_read_u32s(epctx->xhci, sctx->pctx, ctx, sizeof(ctx));
        sct = (ctx[0] >> 1) & 0x07;
        if (epctx->lsa && sct != 1) {
            *cc_error = CC_INVALID_STREAM_TYPE_ERROR;
            return NULL;
        }
        sctx->sct = sct;
        base = xhci_addr64(ctx[0] & ~0xf, ctx[1]);
        xhci_ring_init(epctx->xhci, &sctx->ring, base);
    }
    return sctx;
}
