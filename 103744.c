static bool xhci_get_flag(XHCIState *xhci, enum xhci_flags bit)
{
    return xhci->flags & (1 << bit);
}
