static void xhci_mfwrap_update(XHCIState *xhci)
{
    const uint32_t bits = USBCMD_RS | USBCMD_EWE;
    uint32_t mfindex, left;
    int64_t now;

    if ((xhci->usbcmd & bits) == bits) {
        now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        mfindex = ((now - xhci->mfindex_start) / 125000) & 0x3fff;
        left = 0x4000 - mfindex;
        timer_mod(xhci->mfwrap_timer, now + left * 125000);
    } else {
        timer_del(xhci->mfwrap_timer);
    }
}
