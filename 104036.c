static void ahci_write_fis_sdb(AHCIState *s, int port, uint32_t finished)
{
    AHCIDevice *ad = &s->dev[port];
    AHCIPortRegs *pr = &ad->port_regs;
    IDEState *ide_state;
    SDBFIS *sdb_fis;

    if (!s->dev[port].res_fis ||
        !(pr->cmd & PORT_CMD_FIS_RX)) {
        return;
    }

    sdb_fis = (SDBFIS *)&ad->res_fis[RES_FIS_SDBFIS];
    ide_state = &ad->port.ifs[0];

    sdb_fis->type = 0xA1;
    /* Interrupt pending & Notification bit */
    sdb_fis->flags = (ad->hba->control_regs.irqstatus ? (1 << 6) : 0);
    sdb_fis->status = ide_state->status & 0x77;
    sdb_fis->error = ide_state->error;
    /* update SAct field in SDB_FIS */
    s->dev[port].finished |= finished;
    sdb_fis->payload = cpu_to_le32(ad->finished);

    /* Update shadow registers (except BSY 0x80 and DRQ 0x08) */
    pr->tfdata = (ad->port.ifs[0].error << 8) |
        (ad->port.ifs[0].status & 0x77) |
        (pr->tfdata & 0x88);

    ahci_trigger_irq(s, ad, PORT_IRQ_SDB_FIS);
}
