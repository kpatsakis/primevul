static int xhci_fire_transfer(XHCIState *xhci, XHCITransfer *xfer, XHCIEPContext *epctx)
{
    trace_usb_xhci_xfer_start(xfer, xfer->slotid, xfer->epid, xfer->streamid);
    return xhci_submit(xhci, xfer, epctx);
}
