static uint64_t xhci_mfindex_get(XHCIState *xhci)
{
    int64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    return (now - xhci->mfindex_start) / 125000;
}
