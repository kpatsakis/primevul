static void xhci_calc_intr_kick(XHCIState *xhci, XHCITransfer *xfer,
                                XHCIEPContext *epctx, uint64_t mfindex)
{
    uint64_t asap = ((mfindex + epctx->interval - 1) &
                     ~(epctx->interval-1));
    uint64_t kick = epctx->mfindex_last + epctx->interval;

    assert(epctx->interval != 0);
    xfer->mfindex_kick = MAX(asap, kick);
}
