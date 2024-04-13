static TRBCCode xhci_set_ep_dequeue(XHCIState *xhci, unsigned int slotid,
                                    unsigned int epid, unsigned int streamid,
                                    uint64_t pdequeue)
{
    XHCISlot *slot;
    XHCIEPContext *epctx;
    XHCIStreamContext *sctx;
    dma_addr_t dequeue;

    assert(slotid >= 1 && slotid <= xhci->numslots);

    if (epid < 1 || epid > 31) {
        DPRINTF("xhci: bad ep %d\n", epid);
        return CC_TRB_ERROR;
    }

    trace_usb_xhci_ep_set_dequeue(slotid, epid, streamid, pdequeue);
    dequeue = xhci_mask64(pdequeue);

    slot = &xhci->slots[slotid-1];

    if (!slot->eps[epid-1]) {
        DPRINTF("xhci: slot %d ep %d not enabled\n", slotid, epid);
        return CC_EP_NOT_ENABLED_ERROR;
    }

    epctx = slot->eps[epid-1];

    if (epctx->state != EP_STOPPED) {
        DPRINTF("xhci: set EP dequeue pointer while EP %d not stopped\n", epid);
        return CC_CONTEXT_STATE_ERROR;
    }

    if (epctx->nr_pstreams) {
        uint32_t err;
        sctx = xhci_find_stream(epctx, streamid, &err);
        if (sctx == NULL) {
            return err;
        }
        xhci_ring_init(xhci, &sctx->ring, dequeue & ~0xf);
        sctx->ring.ccs = dequeue & 1;
    } else {
        sctx = NULL;
        xhci_ring_init(xhci, &epctx->ring, dequeue & ~0xF);
        epctx->ring.ccs = dequeue & 1;
    }

    xhci_set_ep_state(xhci, epctx, sctx, EP_STOPPED);

    return CC_SUCCESS;
}
