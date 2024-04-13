static void ahci_write_fis_d2h(AHCIDevice *ad, uint8_t *cmd_fis)
{
    AHCIPortRegs *pr = &ad->port_regs;
    uint8_t *d2h_fis;
    int i;
    dma_addr_t cmd_len = 0x80;
    int cmd_mapped = 0;
    IDEState *s = &ad->port.ifs[0];

    if (!ad->res_fis || !(pr->cmd & PORT_CMD_FIS_RX)) {
        return;
    }

    if (!cmd_fis) {
        /* map cmd_fis */
        uint64_t tbl_addr = le64_to_cpu(ad->cur_cmd->tbl_addr);
        cmd_fis = dma_memory_map(ad->hba->as, tbl_addr, &cmd_len,
                                 DMA_DIRECTION_TO_DEVICE);
        cmd_mapped = 1;
    }

    d2h_fis = &ad->res_fis[RES_FIS_RFIS];

    d2h_fis[0] = 0x34;
    d2h_fis[1] = (ad->hba->control_regs.irqstatus ? (1 << 6) : 0);
    d2h_fis[2] = s->status;
    d2h_fis[3] = s->error;

    d2h_fis[4] = s->sector;
    d2h_fis[5] = s->lcyl;
    d2h_fis[6] = s->hcyl;
    d2h_fis[7] = s->select;
    d2h_fis[8] = s->hob_sector;
    d2h_fis[9] = s->hob_lcyl;
    d2h_fis[10] = s->hob_hcyl;
    d2h_fis[11] = 0;
    d2h_fis[12] = cmd_fis[12];
    d2h_fis[13] = cmd_fis[13];
    for (i = 14; i < 20; i++) {
        d2h_fis[i] = 0;
    }

    /* Update shadow registers: */
    pr->tfdata = (ad->port.ifs[0].error << 8) |
        ad->port.ifs[0].status;

    if (d2h_fis[2] & ERR_STAT) {
        ahci_trigger_irq(ad->hba, ad, PORT_IRQ_TF_ERR);
    }

    ahci_trigger_irq(ad->hba, ad, PORT_IRQ_D2H_REG_FIS);

    if (cmd_mapped) {
        dma_memory_unmap(ad->hba->as, cmd_fis, cmd_len,
                         DMA_DIRECTION_TO_DEVICE, cmd_len);
    }
}
