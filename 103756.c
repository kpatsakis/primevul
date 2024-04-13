static void xhci_msix_update(XHCIState *xhci, int v)
{
    PCIDevice *pci_dev = PCI_DEVICE(xhci);
    bool enabled;

    if (!msix_enabled(pci_dev)) {
        return;
    }

    enabled = xhci->intr[v].iman & IMAN_IE;
    if (enabled == xhci->intr[v].msix_used) {
        return;
    }

    if (enabled) {
        trace_usb_xhci_irq_msix_use(v);
        msix_vector_use(pci_dev, v);
        xhci->intr[v].msix_used = true;
    } else {
        trace_usb_xhci_irq_msix_unuse(v);
        msix_vector_unuse(pci_dev, v);
        xhci->intr[v].msix_used = false;
    }
}
