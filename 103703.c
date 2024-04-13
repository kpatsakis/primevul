static TRBCCode xhci_alloc_device_streams(XHCIState *xhci, unsigned int slotid,
                                          uint32_t epmask)
{
    XHCIEPContext *epctxs[30];
    USBEndpoint *eps[30];
    int i, r, nr_eps, req_nr_streams, dev_max_streams;

    nr_eps = xhci_epmask_to_eps_with_streams(xhci, slotid, epmask, epctxs,
                                             eps);
    if (nr_eps == 0) {
        return CC_SUCCESS;
    }

    req_nr_streams = epctxs[0]->nr_pstreams;
    dev_max_streams = eps[0]->max_streams;

    for (i = 1; i < nr_eps; i++) {
        /*
         * HdG: I don't expect these to ever trigger, but if they do we need
         * to come up with another solution, ie group identical endpoints
         * together and make an usb_device_alloc_streams call per group.
         */
        if (epctxs[i]->nr_pstreams != req_nr_streams) {
            FIXME("guest streams config not identical for all eps");
            return CC_RESOURCE_ERROR;
        }
        if (eps[i]->max_streams != dev_max_streams) {
            FIXME("device streams config not identical for all eps");
            return CC_RESOURCE_ERROR;
        }
    }

    /*
     * max-streams in both the device descriptor and in the controller is a
     * power of 2. But stream id 0 is reserved, so if a device can do up to 4
     * streams the guest will ask for 5 rounded up to the next power of 2 which
     * becomes 8. For emulated devices usb_device_alloc_streams is a nop.
     *
     * For redirected devices however this is an issue, as there we must ask
     * the real xhci controller to alloc streams, and the host driver for the
     * real xhci controller will likely disallow allocating more streams then
     * the device can handle.
     *
     * So we limit the requested nr_streams to the maximum number the device
     * can handle.
     */
    if (req_nr_streams > dev_max_streams) {
        req_nr_streams = dev_max_streams;
    }

    r = usb_device_alloc_streams(eps[0]->dev, eps, nr_eps, req_nr_streams);
    if (r != 0) {
        DPRINTF("xhci: alloc streams failed\n");
        return CC_RESOURCE_ERROR;
    }

    return CC_SUCCESS;
}
