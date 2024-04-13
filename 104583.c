static int xhci_xfer_create_sgl(XHCITransfer *xfer, int in_xfer)
{
    XHCIState *xhci = xfer->xhci;
    int i;

    xfer->int_req = false;
    pci_dma_sglist_init(&xfer->sgl, PCI_DEVICE(xhci), xfer->trb_count);
    for (i = 0; i < xfer->trb_count; i++) {
        XHCITRB *trb = &xfer->trbs[i];
        dma_addr_t addr;
        unsigned int chunk = 0;

        if (trb->control & TRB_TR_IOC) {
            xfer->int_req = true;
        }

        switch (TRB_TYPE(*trb)) {
        case TR_DATA:
            if ((!(trb->control & TRB_TR_DIR)) != (!in_xfer)) {
                DPRINTF("xhci: data direction mismatch for TR_DATA\n");
                goto err;
            }
            /* fallthrough */
        case TR_NORMAL:
        case TR_ISOCH:
            addr = xhci_mask64(trb->parameter);
            chunk = trb->status & 0x1ffff;
            if (trb->control & TRB_TR_IDT) {
                if (chunk > 8 || in_xfer) {
                    DPRINTF("xhci: invalid immediate data TRB\n");
                    goto err;
                }
                qemu_sglist_add(&xfer->sgl, trb->addr, chunk);
            } else {
                qemu_sglist_add(&xfer->sgl, addr, chunk);
            }
            break;
        }
    }

    return 0;

err:
    qemu_sglist_destroy(&xfer->sgl);
    xhci_die(xhci);
    return -1;
}
