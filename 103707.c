static void xhci_attach(USBPort *usbport)
{
    XHCIState *xhci = usbport->opaque;
    XHCIPort *port = xhci_lookup_port(xhci, usbport);

    xhci_port_update(port, 0);
}
