static TRBCCode xhci_evaluate_slot(XHCIState *xhci, unsigned int slotid,
                                   uint64_t pictx)
{
    dma_addr_t ictx, octx;
    uint32_t ictl_ctx[2];
    uint32_t iep0_ctx[5];
    uint32_t ep0_ctx[5];
    uint32_t islot_ctx[4];
    uint32_t slot_ctx[4];

    trace_usb_xhci_slot_evaluate(slotid);
    assert(slotid >= 1 && slotid <= xhci->numslots);

    ictx = xhci_mask64(pictx);
    octx = xhci->slots[slotid-1].ctx;

    DPRINTF("xhci: input context at "DMA_ADDR_FMT"\n", ictx);
    DPRINTF("xhci: output context at "DMA_ADDR_FMT"\n", octx);

    xhci_dma_read_u32s(xhci, ictx, ictl_ctx, sizeof(ictl_ctx));

    if (ictl_ctx[0] != 0x0 || ictl_ctx[1] & ~0x3) {
        DPRINTF("xhci: invalid input context control %08x %08x\n",
                ictl_ctx[0], ictl_ctx[1]);
        return CC_TRB_ERROR;
    }

    if (ictl_ctx[1] & 0x1) {
        xhci_dma_read_u32s(xhci, ictx+32, islot_ctx, sizeof(islot_ctx));

        DPRINTF("xhci: input slot context: %08x %08x %08x %08x\n",
                islot_ctx[0], islot_ctx[1], islot_ctx[2], islot_ctx[3]);

        xhci_dma_read_u32s(xhci, octx, slot_ctx, sizeof(slot_ctx));

        slot_ctx[1] &= ~0xFFFF; /* max exit latency */
        slot_ctx[1] |= islot_ctx[1] & 0xFFFF;
        slot_ctx[2] &= ~0xFF00000; /* interrupter target */
        slot_ctx[2] |= islot_ctx[2] & 0xFF000000;

        DPRINTF("xhci: output slot context: %08x %08x %08x %08x\n",
                slot_ctx[0], slot_ctx[1], slot_ctx[2], slot_ctx[3]);

        xhci_dma_write_u32s(xhci, octx, slot_ctx, sizeof(slot_ctx));
    }

    if (ictl_ctx[1] & 0x2) {
        xhci_dma_read_u32s(xhci, ictx+64, iep0_ctx, sizeof(iep0_ctx));

        DPRINTF("xhci: input ep0 context: %08x %08x %08x %08x %08x\n",
                iep0_ctx[0], iep0_ctx[1], iep0_ctx[2],
                iep0_ctx[3], iep0_ctx[4]);

        xhci_dma_read_u32s(xhci, octx+32, ep0_ctx, sizeof(ep0_ctx));

        ep0_ctx[1] &= ~0xFFFF0000; /* max packet size*/
        ep0_ctx[1] |= iep0_ctx[1] & 0xFFFF0000;

        DPRINTF("xhci: output ep0 context: %08x %08x %08x %08x %08x\n",
                ep0_ctx[0], ep0_ctx[1], ep0_ctx[2], ep0_ctx[3], ep0_ctx[4]);

        xhci_dma_write_u32s(xhci, octx+32, ep0_ctx, sizeof(ep0_ctx));
    }

    return CC_SUCCESS;
}
