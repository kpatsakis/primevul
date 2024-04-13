static void xhci_intx_update(XHCIState *xhci)
{
    PCIDevice *pci_dev = PCI_DEVICE(xhci);
    int level = 0;

    if (msix_enabled(pci_dev) ||
        msi_enabled(pci_dev)) {
        return;
    }

    if (xhci->intr[0].iman & IMAN_IP &&
        xhci->intr[0].iman & IMAN_IE &&
        xhci->usbcmd & USBCMD_INTE) {
        level = 1;
    }

    trace_usb_xhci_irq_intx(level);
    pci_set_irq(pci_dev, level);
}
