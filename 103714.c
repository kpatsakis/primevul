static void xhci_complete(USBPort *port, USBPacket *packet)
{
    XHCITransfer *xfer = container_of(packet, XHCITransfer, packet);

    if (packet->status == USB_RET_REMOVE_FROM_QUEUE) {
        xhci_ep_nuke_one_xfer(xfer, 0);
        return;
    }
    xhci_try_complete_packet(xfer);
    xhci_kick_epctx(xfer->epctx, xfer->streamid);
    if (xfer->complete) {
        xhci_ep_free_xfer(xfer);
    }
}
