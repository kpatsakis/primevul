static void xhci_die(XHCIState *xhci)
{
    xhci->usbsts |= USBSTS_HCE;
    DPRINTF("xhci: asserted controller error\n");
}
