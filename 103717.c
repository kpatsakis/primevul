static void xhci_detach_slot(XHCIState *xhci, USBPort *uport)
{
    int slot, ep;

    for (slot = 0; slot < xhci->numslots; slot++) {
        if (xhci->slots[slot].uport == uport) {
            break;
        }
    }
    if (slot == xhci->numslots) {
        return;
    }

    for (ep = 0; ep < 31; ep++) {
        if (xhci->slots[slot].eps[ep]) {
            xhci_ep_nuke_xfers(xhci, slot + 1, ep + 1, 0);
        }
    }
    xhci->slots[slot].uport = NULL;
}
