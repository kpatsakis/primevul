static TRBCCode xhci_enable_ep(XHCIState *xhci, unsigned int slotid,
                               unsigned int epid, dma_addr_t pctx,
                               uint32_t *ctx)
{
    XHCISlot *slot;
    XHCIEPContext *epctx;

    trace_usb_xhci_ep_enable(slotid, epid);
    assert(slotid >= 1 && slotid <= xhci->numslots);
    assert(epid >= 1 && epid <= 31);

    slot = &xhci->slots[slotid-1];
    if (slot->eps[epid-1]) {
        xhci_disable_ep(xhci, slotid, epid);
    }

    epctx = xhci_alloc_epctx(xhci, slotid, epid);
    slot->eps[epid-1] = epctx;
    xhci_init_epctx(epctx, pctx, ctx);

    DPRINTF("xhci: endpoint %d.%d type is %d, max transaction (burst) "
            "size is %d\n", epid/2, epid%2, epctx->type, epctx->max_psize);

    epctx->mfindex_last = 0;

    epctx->state = EP_RUNNING;
    ctx[0] &= ~EP_STATE_MASK;
    ctx[0] |= EP_RUNNING;

    return CC_SUCCESS;
}
