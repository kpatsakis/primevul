static XHCIEPContext *xhci_alloc_epctx(XHCIState *xhci,
                                       unsigned int slotid,
                                       unsigned int epid)
{
    XHCIEPContext *epctx;

    epctx = g_new0(XHCIEPContext, 1);
    epctx->xhci = xhci;
    epctx->slotid = slotid;
    epctx->epid = epid;

    QTAILQ_INIT(&epctx->transfers);
    epctx->kick_timer = timer_new_ns(QEMU_CLOCK_VIRTUAL, xhci_ep_kick_timer, epctx);

    return epctx;
}
