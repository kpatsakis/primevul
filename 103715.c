static TRBCCode xhci_configure_slot(XHCIState *xhci, unsigned int slotid,
                                  uint64_t pictx, bool dc)
{
    dma_addr_t ictx, octx;
    uint32_t ictl_ctx[2];
    uint32_t slot_ctx[4];
    uint32_t islot_ctx[4];
    uint32_t ep_ctx[5];
    int i;
    TRBCCode res;

    trace_usb_xhci_slot_configure(slotid);
    assert(slotid >= 1 && slotid <= xhci->numslots);

    ictx = xhci_mask64(pictx);
    octx = xhci->slots[slotid-1].ctx;

    DPRINTF("xhci: input context at "DMA_ADDR_FMT"\n", ictx);
    DPRINTF("xhci: output context at "DMA_ADDR_FMT"\n", octx);

    if (dc) {
        for (i = 2; i <= 31; i++) {
            if (xhci->slots[slotid-1].eps[i-1]) {
                xhci_disable_ep(xhci, slotid, i);
            }
        }

        xhci_dma_read_u32s(xhci, octx, slot_ctx, sizeof(slot_ctx));
        slot_ctx[3] &= ~(SLOT_STATE_MASK << SLOT_STATE_SHIFT);
        slot_ctx[3] |= SLOT_ADDRESSED << SLOT_STATE_SHIFT;
        DPRINTF("xhci: output slot context: %08x %08x %08x %08x\n",
                slot_ctx[0], slot_ctx[1], slot_ctx[2], slot_ctx[3]);
        xhci_dma_write_u32s(xhci, octx, slot_ctx, sizeof(slot_ctx));

        return CC_SUCCESS;
    }

    xhci_dma_read_u32s(xhci, ictx, ictl_ctx, sizeof(ictl_ctx));

    if ((ictl_ctx[0] & 0x3) != 0x0 || (ictl_ctx[1] & 0x3) != 0x1) {
        DPRINTF("xhci: invalid input context control %08x %08x\n",
                ictl_ctx[0], ictl_ctx[1]);
        return CC_TRB_ERROR;
    }

    xhci_dma_read_u32s(xhci, ictx+32, islot_ctx, sizeof(islot_ctx));
    xhci_dma_read_u32s(xhci, octx, slot_ctx, sizeof(slot_ctx));

    if (SLOT_STATE(slot_ctx[3]) < SLOT_ADDRESSED) {
        DPRINTF("xhci: invalid slot state %08x\n", slot_ctx[3]);
        return CC_CONTEXT_STATE_ERROR;
    }

    xhci_free_device_streams(xhci, slotid, ictl_ctx[0] | ictl_ctx[1]);

    for (i = 2; i <= 31; i++) {
        if (ictl_ctx[0] & (1<<i)) {
            xhci_disable_ep(xhci, slotid, i);
        }
        if (ictl_ctx[1] & (1<<i)) {
            xhci_dma_read_u32s(xhci, ictx+32+(32*i), ep_ctx, sizeof(ep_ctx));
            DPRINTF("xhci: input ep%d.%d context: %08x %08x %08x %08x %08x\n",
                    i/2, i%2, ep_ctx[0], ep_ctx[1], ep_ctx[2],
                    ep_ctx[3], ep_ctx[4]);
            xhci_disable_ep(xhci, slotid, i);
            res = xhci_enable_ep(xhci, slotid, i, octx+(32*i), ep_ctx);
            if (res != CC_SUCCESS) {
                return res;
            }
            DPRINTF("xhci: output ep%d.%d context: %08x %08x %08x %08x %08x\n",
                    i/2, i%2, ep_ctx[0], ep_ctx[1], ep_ctx[2],
                    ep_ctx[3], ep_ctx[4]);
            xhci_dma_write_u32s(xhci, octx+(32*i), ep_ctx, sizeof(ep_ctx));
        }
    }

    res = xhci_alloc_device_streams(xhci, slotid, ictl_ctx[1]);
    if (res != CC_SUCCESS) {
        for (i = 2; i <= 31; i++) {
            if (ictl_ctx[1] & (1u << i)) {
                xhci_disable_ep(xhci, slotid, i);
            }
        }
        return res;
    }

    slot_ctx[3] &= ~(SLOT_STATE_MASK << SLOT_STATE_SHIFT);
    slot_ctx[3] |= SLOT_CONFIGURED << SLOT_STATE_SHIFT;
    slot_ctx[0] &= ~(SLOT_CONTEXT_ENTRIES_MASK << SLOT_CONTEXT_ENTRIES_SHIFT);
    slot_ctx[0] |= islot_ctx[0] & (SLOT_CONTEXT_ENTRIES_MASK <<
                                   SLOT_CONTEXT_ENTRIES_SHIFT);
    DPRINTF("xhci: output slot context: %08x %08x %08x %08x\n",
            slot_ctx[0], slot_ctx[1], slot_ctx[2], slot_ctx[3]);

    xhci_dma_write_u32s(xhci, octx, slot_ctx, sizeof(slot_ctx));

    return CC_SUCCESS;
}
