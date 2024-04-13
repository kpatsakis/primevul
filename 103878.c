static void ahci_check_irq(AHCIState *s)
{
    int i;

    DPRINTF(-1, "check irq %#x\n", s->control_regs.irqstatus);

    s->control_regs.irqstatus = 0;
    for (i = 0; i < s->ports; i++) {
        AHCIPortRegs *pr = &s->dev[i].port_regs;
        if (pr->irq_stat & pr->irq_mask) {
            s->control_regs.irqstatus |= (1 << i);
        }
    }

    if (s->control_regs.irqstatus &&
        (s->control_regs.ghc & HOST_CTL_IRQ_EN)) {
            ahci_irq_raise(s, NULL);
    } else {
        ahci_irq_lower(s, NULL);
    }
}
