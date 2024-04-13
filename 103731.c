static USBEndpoint *xhci_epid_to_usbep(XHCIEPContext *epctx)
{
    USBPort *uport;
    uint32_t token;

    if (!epctx) {
        return NULL;
    }
    uport = epctx->xhci->slots[epctx->slotid - 1].uport;
    token = (epctx->epid & 1) ? USB_TOKEN_IN : USB_TOKEN_OUT;
    if (!uport) {
        return NULL;
    }
    return usb_ep_get(uport->dev, token, epctx->epid >> 1);
}
