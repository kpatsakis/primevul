static void ahci_write_fis_pio(AHCIDevice *ad, uint16_t len)
{
    AHCIPortRegs *pr = &ad->port_regs;
    uint8_t *pio_fis;
    IDEState *s = &ad->port.ifs[0];

    if (!ad->res_fis || !(pr->cmd & PORT_CMD_FIS_RX)) {
        return;
    }

    pio_fis = &ad->res_fis[RES_FIS_PSFIS];

    pio_fis[0] = SATA_FIS_TYPE_PIO_SETUP;
    pio_fis[1] = (ad->hba->control_regs.irqstatus ? (1 << 6) : 0);
    pio_fis[2] = s->status;
    pio_fis[3] = s->error;

    pio_fis[4] = s->sector;
    pio_fis[5] = s->lcyl;
    pio_fis[6] = s->hcyl;
    pio_fis[7] = s->select;
    pio_fis[8] = s->hob_sector;
    pio_fis[9] = s->hob_lcyl;
    pio_fis[10] = s->hob_hcyl;
    pio_fis[11] = 0;
    pio_fis[12] = s->nsector & 0xFF;
    pio_fis[13] = (s->nsector >> 8) & 0xFF;
    pio_fis[14] = 0;
    pio_fis[15] = s->status;
    pio_fis[16] = len & 255;
    pio_fis[17] = len >> 8;
    pio_fis[18] = 0;
    pio_fis[19] = 0;

    /* Update shadow registers: */
    pr->tfdata = (ad->port.ifs[0].error << 8) |
        ad->port.ifs[0].status;

    if (pio_fis[2] & ERR_STAT) {
        ahci_trigger_irq(ad->hba, ad, PORT_IRQ_TF_ERR);
    }

    ahci_trigger_irq(ad->hba, ad, PORT_IRQ_PIOS_FIS);
}
