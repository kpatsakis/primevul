static void xhci_ep_free_xfer(XHCITransfer *xfer)
{
    QTAILQ_REMOVE(&xfer->epctx->transfers, xfer, next);
    xfer->epctx->xfer_count--;

    usb_packet_cleanup(&xfer->packet);
    g_free(xfer->trbs);
    g_free(xfer);
}
