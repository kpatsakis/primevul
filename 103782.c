static TRBCCode xhci_stop_ep(XHCIState *xhci, unsigned int slotid,
                             unsigned int epid)
{
    XHCISlot *slot;
    XHCIEPContext *epctx;

    trace_usb_xhci_ep_stop(slotid, epid);
    assert(slotid >= 1 && slotid <= xhci->numslots);

    if (epid < 1 || epid > 31) {
        DPRINTF("xhci: bad ep %d\n", epid);
        return CC_TRB_ERROR;
    }

    slot = &xhci->slots[slotid-1];

    if (!slot->eps[epid-1]) {
        DPRINTF("xhci: slot %d ep %d not enabled\n", slotid, epid);
        return CC_EP_NOT_ENABLED_ERROR;
    }

    if (xhci_ep_nuke_xfers(xhci, slotid, epid, CC_STOPPED) > 0) {
        DPRINTF("xhci: FIXME: endpoint stopped w/ xfers running, "
                "data might be lost\n");
    }

    epctx = slot->eps[epid-1];

    xhci_set_ep_state(xhci, epctx, NULL, EP_STOPPED);

    if (epctx->nr_pstreams) {
        xhci_reset_streams(epctx);
    }

    return CC_SUCCESS;
}
