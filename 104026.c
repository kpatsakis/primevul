static void ahci_irq_lower(AHCIState *s, AHCIDevice *dev)
{
    AHCIPCIState *d = container_of(s, AHCIPCIState, ahci);
    PCIDevice *pci_dev =
        (PCIDevice *)object_dynamic_cast(OBJECT(d), TYPE_PCI_DEVICE);

    DPRINTF(0, "lower irq\n");

    if (!pci_dev || !msi_enabled(pci_dev)) {
        qemu_irq_lower(s->irq);
    }
}
