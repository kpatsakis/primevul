static void xhci_stop(XHCIState *xhci)
{
    trace_usb_xhci_stop();
    xhci->usbsts |= USBSTS_HCH;
    xhci->crcr_low &= ~CRCR_CRR;
}
