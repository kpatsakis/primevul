static void xhci_stall_ep(XHCITransfer *xfer)
{
    XHCIEPContext *epctx = xfer->epctx;
    XHCIState *xhci = epctx->xhci;
    uint32_t err;
    XHCIStreamContext *sctx;

    if (epctx->nr_pstreams) {
        sctx = xhci_find_stream(epctx, xfer->streamid, &err);
        if (sctx == NULL) {
            return;
        }
        sctx->ring.dequeue = xfer->trbs[0].addr;
        sctx->ring.ccs = xfer->trbs[0].ccs;
        xhci_set_ep_state(xhci, epctx, sctx, EP_HALTED);
    } else {
        epctx->ring.dequeue = xfer->trbs[0].addr;
        epctx->ring.ccs = xfer->trbs[0].ccs;
        xhci_set_ep_state(xhci, epctx, NULL, EP_HALTED);
    }
}
