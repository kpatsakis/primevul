static void ahci_trigger_irq(AHCIState *s, AHCIDevice *d,
                             int irq_type)
{
    DPRINTF(d->port_no, "trigger irq %#x -> %x\n",
            irq_type, d->port_regs.irq_mask & irq_type);

    d->port_regs.irq_stat |= irq_type;
    ahci_check_irq(s);
}
