static TRBCCode xhci_enable_slot(XHCIState *xhci, unsigned int slotid)
{
    trace_usb_xhci_slot_enable(slotid);
    assert(slotid >= 1 && slotid <= xhci->numslots);
    xhci->slots[slotid-1].enabled = 1;
    xhci->slots[slotid-1].uport = NULL;
    memset(xhci->slots[slotid-1].eps, 0, sizeof(XHCIEPContext*)*31);

    return CC_SUCCESS;
}
