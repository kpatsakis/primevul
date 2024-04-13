static void pci_ne2000_realize(PCIDevice *pci_dev, Error **errp)
{
    PCINE2000State *d = DO_UPCAST(PCINE2000State, dev, pci_dev);
    NE2000State *s;
    uint8_t *pci_conf;

    pci_conf = d->dev.config;
    pci_conf[PCI_INTERRUPT_PIN] = 1; /* interrupt pin A */

    s = &d->ne2000;
    ne2000_setup_io(s, DEVICE(pci_dev), 0x100);
    pci_register_bar(&d->dev, 0, PCI_BASE_ADDRESS_SPACE_IO, &s->io);
    s->irq = pci_allocate_irq(&d->dev);

    qemu_macaddr_default_if_unset(&s->c.macaddr);
    ne2000_reset(s);

    s->nic = qemu_new_nic(&net_ne2000_info, &s->c,
                          object_get_typename(OBJECT(pci_dev)), pci_dev->qdev.id, s);
    qemu_format_nic_info_str(qemu_get_queue(s->nic), s->c.macaddr.a);
}
