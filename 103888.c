static void ahci_irq_lower(AHCIState *s, AHCIDevice *dev)
{
    DeviceState *dev_state = s->container;
    PCIDevice *pci_dev = (PCIDevice *) object_dynamic_cast(OBJECT(dev_state),
                                                           TYPE_PCI_DEVICE);

    DPRINTF(0, "lower irq\n");

    if (!pci_dev || !msi_enabled(pci_dev)) {
        qemu_irq_lower(s->irq);
    }
}
