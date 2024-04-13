static void xhci_xfer_unmap(XHCITransfer *xfer)
{
    usb_packet_unmap(&xfer->packet, &xfer->sgl);
    qemu_sglist_destroy(&xfer->sgl);
}
