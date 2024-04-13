static XHCIPort *xhci_lookup_port(XHCIState *xhci, struct USBPort *uport)
{
    int index;

    if (!uport->dev) {
        return NULL;
    }
    switch (uport->dev->speed) {
    case USB_SPEED_LOW:
    case USB_SPEED_FULL:
    case USB_SPEED_HIGH:
        if (xhci_get_flag(xhci, XHCI_FLAG_SS_FIRST)) {
            index = uport->index + xhci->numports_3;
        } else {
            index = uport->index;
        }
        break;
    case USB_SPEED_SUPER:
        if (xhci_get_flag(xhci, XHCI_FLAG_SS_FIRST)) {
            index = uport->index;
        } else {
            index = uport->index + xhci->numports_2;
        }
        break;
    default:
        return NULL;
    }
    return &xhci->ports[index];
}
