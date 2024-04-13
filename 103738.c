static int xhci_find_epid(USBEndpoint *ep)
{
    if (ep->nr == 0) {
        return 1;
    }
    if (ep->pid == USB_TOKEN_IN) {
        return ep->nr * 2 + 1;
    } else {
        return ep->nr * 2;
    }
}
