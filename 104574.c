static int xhci_epmask_to_eps_with_streams(XHCIState *xhci,
                                           unsigned int slotid,
                                           uint32_t epmask,
                                           XHCIEPContext **epctxs,
                                           USBEndpoint **eps)
{
    XHCISlot *slot;
    XHCIEPContext *epctx;
    USBEndpoint *ep;
    int i, j;

    assert(slotid >= 1 && slotid <= xhci->numslots);

    slot = &xhci->slots[slotid - 1];

    for (i = 2, j = 0; i <= 31; i++) {
        if (!(epmask & (1u << i))) {
            continue;
        }

        epctx = slot->eps[i - 1];
        ep = xhci_epid_to_usbep(xhci, slotid, i);
        if (!epctx || !epctx->nr_pstreams || !ep) {
            continue;
        }

        if (epctxs) {
            epctxs[j] = epctx;
        }
        eps[j++] = ep;
    }
    return j;
}
