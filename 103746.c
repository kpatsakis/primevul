static unsigned int xhci_get_slot(XHCIState *xhci, XHCIEvent *event, XHCITRB *trb)
{
    unsigned int slotid;
    slotid = (trb->control >> TRB_CR_SLOTID_SHIFT) & TRB_CR_SLOTID_MASK;
    if (slotid < 1 || slotid > xhci->numslots) {
        DPRINTF("xhci: bad slot id %d\n", slotid);
        event->ccode = CC_TRB_ERROR;
        return 0;
    } else if (!xhci->slots[slotid-1].enabled) {
        DPRINTF("xhci: slot id %d not enabled\n", slotid);
        event->ccode = CC_SLOT_NOT_ENABLED_ERROR;
        return 0;
    }
    return slotid;
}
