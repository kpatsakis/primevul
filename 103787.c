static void xhci_wakeup_endpoint(USBBus *bus, USBEndpoint *ep,
                                 unsigned int stream)
{
    XHCIState *xhci = container_of(bus, XHCIState, bus);
    int slotid;

    DPRINTF("%s\n", __func__);
    slotid = ep->dev->addr;
    if (slotid == 0 || !xhci->slots[slotid-1].enabled) {
        DPRINTF("%s: oops, no slot for dev %d\n", __func__, ep->dev->addr);
        return;
    }
    xhci_kick_ep(xhci, slotid, xhci_find_epid(ep), stream);
}
