static XHCITransfer *xhci_ep_alloc_xfer(XHCIEPContext *epctx,
                                        uint32_t length)
{
    uint32_t limit = epctx->nr_pstreams + 16;
    XHCITransfer *xfer;

    if (epctx->xfer_count >= limit) {
        return NULL;
    }

    xfer = g_new0(XHCITransfer, 1);
    xfer->epctx = epctx;
    xfer->trbs = g_new(XHCITRB, length);
    xfer->trb_count = length;
    usb_packet_init(&xfer->packet);

    QTAILQ_INSERT_TAIL(&epctx->transfers, xfer, next);
    epctx->xfer_count++;

    return xfer;
}
