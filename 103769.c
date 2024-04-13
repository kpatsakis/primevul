static TRBCCode xhci_reset_slot(XHCIState *xhci, unsigned int slotid)
{
    uint32_t slot_ctx[4];
    dma_addr_t octx;
    int i;

    trace_usb_xhci_slot_reset(slotid);
    assert(slotid >= 1 && slotid <= xhci->numslots);

    octx = xhci->slots[slotid-1].ctx;

    DPRINTF("xhci: output context at "DMA_ADDR_FMT"\n", octx);

    for (i = 2; i <= 31; i++) {
        if (xhci->slots[slotid-1].eps[i-1]) {
            xhci_disable_ep(xhci, slotid, i);
        }
    }

    xhci_dma_read_u32s(xhci, octx, slot_ctx, sizeof(slot_ctx));
    slot_ctx[3] &= ~(SLOT_STATE_MASK << SLOT_STATE_SHIFT);
    slot_ctx[3] |= SLOT_DEFAULT << SLOT_STATE_SHIFT;
    DPRINTF("xhci: output slot context: %08x %08x %08x %08x\n",
            slot_ctx[0], slot_ctx[1], slot_ctx[2], slot_ctx[3]);
    xhci_dma_write_u32s(xhci, octx, slot_ctx, sizeof(slot_ctx));

    return CC_SUCCESS;
}
