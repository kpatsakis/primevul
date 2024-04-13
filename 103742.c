static void xhci_free_device_streams(XHCIState *xhci, unsigned int slotid,
                                     uint32_t epmask)
{
    USBEndpoint *eps[30];
    int nr_eps;

    nr_eps = xhci_epmask_to_eps_with_streams(xhci, slotid, epmask, NULL, eps);
    if (nr_eps) {
        usb_device_free_streams(eps[0]->dev, eps, nr_eps);
    }
}
