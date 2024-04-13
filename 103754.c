static void xhci_mfwrap_timer(void *opaque)
{
    XHCIState *xhci = opaque;
    XHCIEvent wrap = { ER_MFINDEX_WRAP, CC_SUCCESS };

    xhci_event(xhci, &wrap, 0);
    xhci_mfwrap_update(xhci);
}
