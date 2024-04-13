static void xhci_complete(USBPort *port, USBPacket *packet)
{
    XHCITransfer *xfer = container_of(packet, XHCITransfer, packet);

    if (packet->status == USB_RET_REMOVE_FROM_QUEUE) {
        xhci_ep_nuke_one_xfer(xfer, 0);
        return;
    }
    xhci_complete_packet(xfer);
    xhci_kick_ep(xfer->xhci, xfer->slotid, xfer->epid, xfer->streamid);
}
