static int xhci_ep_nuke_xfers(XHCIState *xhci, unsigned int slotid,
                               unsigned int epid, TRBCCode report)
{
    XHCISlot *slot;
    XHCIEPContext *epctx;
    XHCITransfer *xfer;
    int killed = 0;
    USBEndpoint *ep = NULL;
    assert(slotid >= 1 && slotid <= xhci->numslots);
    assert(epid >= 1 && epid <= 31);

    DPRINTF("xhci_ep_nuke_xfers(%d, %d)\n", slotid, epid);

    slot = &xhci->slots[slotid-1];

    if (!slot->eps[epid-1]) {
        return 0;
    }

    epctx = slot->eps[epid-1];

    for (;;) {
        xfer = QTAILQ_FIRST(&epctx->transfers);
        if (xfer == NULL) {
            break;
        }
        killed += xhci_ep_nuke_one_xfer(xfer, report);
        if (killed) {
            report = 0; /* Only report once */
        }
        xhci_ep_free_xfer(xfer);
    }

    ep = xhci_epid_to_usbep(epctx);
    if (ep) {
        usb_device_ep_stopped(ep->dev, ep);
    }
    return killed;
}
