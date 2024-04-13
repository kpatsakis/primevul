static void xhci_run(XHCIState *xhci)
{
    trace_usb_xhci_run();
    xhci->usbsts &= ~USBSTS_HCH;
    xhci->mfindex_start = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
}
