static void lsi_set_irq(LSIState *s, int level)
{
    PCIDevice *d = PCI_DEVICE(s);

    if (s->ext_irq) {
        qemu_set_irq(s->ext_irq, level);
    } else {
        pci_set_irq(d, level);
    }
}
