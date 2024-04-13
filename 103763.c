static void xhci_port_update(XHCIPort *port, int is_detach)
{
    uint32_t pls = PLS_RX_DETECT;

    port->portsc = PORTSC_PP;
    if (!is_detach && xhci_port_have_device(port)) {
        port->portsc |= PORTSC_CCS;
        switch (port->uport->dev->speed) {
        case USB_SPEED_LOW:
            port->portsc |= PORTSC_SPEED_LOW;
            pls = PLS_POLLING;
            break;
        case USB_SPEED_FULL:
            port->portsc |= PORTSC_SPEED_FULL;
            pls = PLS_POLLING;
            break;
        case USB_SPEED_HIGH:
            port->portsc |= PORTSC_SPEED_HIGH;
            pls = PLS_POLLING;
            break;
        case USB_SPEED_SUPER:
            port->portsc |= PORTSC_SPEED_SUPER;
            port->portsc |= PORTSC_PED;
            pls = PLS_U0;
            break;
        }
    }
    set_field(&port->portsc, pls, PORTSC_PLS);
    trace_usb_xhci_port_link(port->portnr, pls);
    xhci_port_notify(port, PORTSC_CSC);
}
