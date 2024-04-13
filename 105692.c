static void pci_ne2000_exit(PCIDevice *pci_dev)
{
    PCINE2000State *d = DO_UPCAST(PCINE2000State, dev, pci_dev);
    NE2000State *s = &d->ne2000;

    qemu_del_nic(s->nic);
    qemu_free_irq(s->irq);
}
