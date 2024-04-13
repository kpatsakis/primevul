static bool xhci_port_have_device(XHCIPort *port)
{
    if (!port->uport->dev || !port->uport->dev->attached) {
        return false; /* no device present */
    }
    if (!((1 << port->uport->dev->speed) & port->speedmask)) {
        return false; /* speed mismatch */
    }
    return true;
}
