static void xhci_detach(USBPort *usbport)
{
    XHCIState *xhci = usbport->opaque;
    XHCIPort *port = xhci_lookup_port(xhci, usbport);

    xhci_detach_slot(xhci, usbport);
    xhci_port_update(port, 1);
}
