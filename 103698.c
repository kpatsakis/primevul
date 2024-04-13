static void usb_xhci_init(XHCIState *xhci)
{
    DeviceState *dev = DEVICE(xhci);
    XHCIPort *port;
    int i, usbports, speedmask;

    xhci->usbsts = USBSTS_HCH;

    if (xhci->numports_2 > MAXPORTS_2) {
        xhci->numports_2 = MAXPORTS_2;
    }
    if (xhci->numports_3 > MAXPORTS_3) {
        xhci->numports_3 = MAXPORTS_3;
    }
    usbports = MAX(xhci->numports_2, xhci->numports_3);
    xhci->numports = xhci->numports_2 + xhci->numports_3;

    usb_bus_new(&xhci->bus, sizeof(xhci->bus), &xhci_bus_ops, dev);

    for (i = 0; i < usbports; i++) {
        speedmask = 0;
        if (i < xhci->numports_2) {
            if (xhci_get_flag(xhci, XHCI_FLAG_SS_FIRST)) {
                port = &xhci->ports[i + xhci->numports_3];
                port->portnr = i + 1 + xhci->numports_3;
            } else {
                port = &xhci->ports[i];
                port->portnr = i + 1;
            }
            port->uport = &xhci->uports[i];
            port->speedmask =
                USB_SPEED_MASK_LOW  |
                USB_SPEED_MASK_FULL |
                USB_SPEED_MASK_HIGH;
            snprintf(port->name, sizeof(port->name), "usb2 port #%d", i+1);
            speedmask |= port->speedmask;
        }
        if (i < xhci->numports_3) {
            if (xhci_get_flag(xhci, XHCI_FLAG_SS_FIRST)) {
                port = &xhci->ports[i];
                port->portnr = i + 1;
            } else {
                port = &xhci->ports[i + xhci->numports_2];
                port->portnr = i + 1 + xhci->numports_2;
            }
            port->uport = &xhci->uports[i];
            port->speedmask = USB_SPEED_MASK_SUPER;
            snprintf(port->name, sizeof(port->name), "usb3 port #%d", i+1);
            speedmask |= port->speedmask;
        }
        usb_register_port(&xhci->bus, &xhci->uports[i], xhci, i,
                          &xhci_uport_ops, speedmask);
    }
}
