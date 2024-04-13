static void xhci_write_event(XHCIState *xhci, XHCIEvent *event, int v)
{
    PCIDevice *pci_dev = PCI_DEVICE(xhci);
    XHCIInterrupter *intr = &xhci->intr[v];
    XHCITRB ev_trb;
    dma_addr_t addr;

    ev_trb.parameter = cpu_to_le64(event->ptr);
    ev_trb.status = cpu_to_le32(event->length | (event->ccode << 24));
    ev_trb.control = (event->slotid << 24) | (event->epid << 16) |
                     event->flags | (event->type << TRB_TYPE_SHIFT);
    if (intr->er_pcs) {
        ev_trb.control |= TRB_C;
    }
    ev_trb.control = cpu_to_le32(ev_trb.control);

    trace_usb_xhci_queue_event(v, intr->er_ep_idx, trb_name(&ev_trb),
                               event_name(event), ev_trb.parameter,
                               ev_trb.status, ev_trb.control);

    addr = intr->er_start + TRB_SIZE*intr->er_ep_idx;
    pci_dma_write(pci_dev, addr, &ev_trb, TRB_SIZE);

    intr->er_ep_idx++;
    if (intr->er_ep_idx >= intr->er_size) {
        intr->er_ep_idx = 0;
        intr->er_pcs = !intr->er_pcs;
    }
}
