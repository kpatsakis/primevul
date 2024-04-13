static void xhci_wakeup(USBPort *usbport)
{
    XHCIState *xhci = usbport->opaque;
    XHCIPort *port = xhci_lookup_port(xhci, usbport);

    if (get_field(port->portsc, PORTSC_PLS) != PLS_U3) {
        return;
    }
    set_field(&port->portsc, PLS_RESUME, PORTSC_PLS);
    xhci_port_notify(port, PORTSC_PLC);
}
