static void xhci_set_ep_state(XHCIState *xhci, XHCIEPContext *epctx,
                              XHCIStreamContext *sctx, uint32_t state)
{
    XHCIRing *ring = NULL;
    uint32_t ctx[5];
    uint32_t ctx2[2];

    xhci_dma_read_u32s(xhci, epctx->pctx, ctx, sizeof(ctx));
    ctx[0] &= ~EP_STATE_MASK;
    ctx[0] |= state;

    /* update ring dequeue ptr */
    if (epctx->nr_pstreams) {
        if (sctx != NULL) {
            ring = &sctx->ring;
            xhci_dma_read_u32s(xhci, sctx->pctx, ctx2, sizeof(ctx2));
            ctx2[0] &= 0xe;
            ctx2[0] |= sctx->ring.dequeue | sctx->ring.ccs;
            ctx2[1] = (sctx->ring.dequeue >> 16) >> 16;
            xhci_dma_write_u32s(xhci, sctx->pctx, ctx2, sizeof(ctx2));
        }
    } else {
        ring = &epctx->ring;
    }
    if (ring) {
        ctx[2] = ring->dequeue | ring->ccs;
        ctx[3] = (ring->dequeue >> 16) >> 16;

        DPRINTF("xhci: set epctx: " DMA_ADDR_FMT " state=%d dequeue=%08x%08x\n",
                epctx->pctx, state, ctx[3], ctx[2]);
    }

    xhci_dma_write_u32s(xhci, epctx->pctx, ctx, sizeof(ctx));
    if (epctx->state != state) {
        trace_usb_xhci_ep_state(epctx->slotid, epctx->epid,
                                ep_state_name(epctx->state),
                                ep_state_name(state));
    }
    epctx->state = state;
}
