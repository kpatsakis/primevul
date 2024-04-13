static int xhci_setup_packet(XHCITransfer *xfer)
{
    XHCIState *xhci = xfer->xhci;
    USBEndpoint *ep;
    int dir;

    dir = xfer->in_xfer ? USB_TOKEN_IN : USB_TOKEN_OUT;

    if (xfer->packet.ep) {
        ep = xfer->packet.ep;
    } else {
        ep = xhci_epid_to_usbep(xhci, xfer->slotid, xfer->epid);
        if (!ep) {
            DPRINTF("xhci: slot %d has no device\n",
                    xfer->slotid);
            return -1;
        }
    }

    xhci_xfer_create_sgl(xfer, dir == USB_TOKEN_IN); /* Also sets int_req */
    usb_packet_setup(&xfer->packet, dir, ep, xfer->streamid,
                     xfer->trbs[0].addr, false, xfer->int_req);
    usb_packet_map(&xfer->packet, &xfer->sgl);
    DPRINTF("xhci: setup packet pid 0x%x addr %d ep %d\n",
            xfer->packet.pid, ep->dev->addr, ep->nr);
    return 0;
}
