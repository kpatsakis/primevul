static void ahci_write_fis_pio(AHCIDevice *ad, uint16_t len)
{
    AHCIPortRegs *pr = &ad->port_regs;
    uint8_t *pio_fis, *cmd_fis;
    uint64_t tbl_addr;
    dma_addr_t cmd_len = 0x80;
    IDEState *s = &ad->port.ifs[0];

    if (!ad->res_fis || !(pr->cmd & PORT_CMD_FIS_RX)) {
        return;
    }

    /* map cmd_fis */
    tbl_addr = le64_to_cpu(ad->cur_cmd->tbl_addr);
    cmd_fis = dma_memory_map(ad->hba->as, tbl_addr, &cmd_len,
                             DMA_DIRECTION_TO_DEVICE);

    if (cmd_fis == NULL) {
        DPRINTF(ad->port_no, "dma_memory_map failed in ahci_write_fis_pio");
        ahci_trigger_irq(ad->hba, ad, PORT_IRQ_HBUS_ERR);
        return;
    }

    if (cmd_len != 0x80) {
        DPRINTF(ad->port_no,
                "dma_memory_map mapped too few bytes in ahci_write_fis_pio");
        dma_memory_unmap(ad->hba->as, cmd_fis, cmd_len,
                         DMA_DIRECTION_TO_DEVICE, cmd_len);
        ahci_trigger_irq(ad->hba, ad, PORT_IRQ_HBUS_ERR);
        return;
    }

    pio_fis = &ad->res_fis[RES_FIS_PSFIS];

    pio_fis[0] = 0x5f;
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
    pio_fis[12] = cmd_fis[12];
    pio_fis[13] = cmd_fis[13];
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

    dma_memory_unmap(ad->hba->as, cmd_fis, cmd_len,
                     DMA_DIRECTION_TO_DEVICE, cmd_len);
}
