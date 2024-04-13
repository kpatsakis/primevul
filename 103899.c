void ahci_realize(AHCIState *s, DeviceState *qdev, AddressSpace *as, int ports)
{
    qemu_irq *irqs;
    int i;

    s->as = as;
    s->ports = ports;
    s->dev = g_new0(AHCIDevice, ports);
    ahci_reg_init(s);
    irqs = qemu_allocate_irqs(ahci_irq_set, s, s->ports);
    for (i = 0; i < s->ports; i++) {
        AHCIDevice *ad = &s->dev[i];

        ide_bus_new(&ad->port, sizeof(ad->port), qdev, i, 1);
        ide_init2(&ad->port, irqs[i]);

        ad->hba = s;
        ad->port_no = i;
        ad->port.dma = &ad->dma;
        ad->port.dma->ops = &ahci_dma_ops;
        ide_register_restart_cb(&ad->port);
    }
    g_free(irqs);
}
