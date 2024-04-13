static USBEndpoint *xhci_epid_to_usbep(XHCIState *xhci,
                                       unsigned int slotid, unsigned int epid)
{
    assert(slotid >= 1 && slotid <= xhci->numslots);

    if (!xhci->slots[slotid - 1].uport) {
        return NULL;
    }

    return usb_ep_get(xhci->slots[slotid - 1].uport->dev,
                      (epid & 1) ? USB_TOKEN_IN : USB_TOKEN_OUT, epid >> 1);
}
