static void ahci_irq_raise(AHCIState *s, AHCIDevice *dev)
{
    DeviceState *dev_state = s->container;
    PCIDevice *pci_dev = (PCIDevice *) object_dynamic_cast(OBJECT(dev_state),
                                                           TYPE_PCI_DEVICE);

    DPRINTF(0, "raise irq\n");

    if (pci_dev && msi_enabled(pci_dev)) {
        msi_notify(pci_dev, 0);
    } else {
        qemu_irq_raise(s->irq);
    }
}
