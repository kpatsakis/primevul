static int handle_cmd(AHCIState *s, int port, uint8_t slot)
{
    IDEState *ide_state;
    uint64_t tbl_addr;
    AHCICmdHdr *cmd;
    uint8_t *cmd_fis;
    dma_addr_t cmd_len;

    if (s->dev[port].port.ifs[0].status & (BUSY_STAT|DRQ_STAT)) {
        /* Engine currently busy, try again later */
        DPRINTF(port, "engine busy\n");
        return -1;
    }

    if (!s->dev[port].lst) {
        DPRINTF(port, "error: lst not given but cmd handled");
        return -1;
    }
    cmd = get_cmd_header(s, port, slot);
    /* remember current slot handle for later */
    s->dev[port].cur_cmd = cmd;

    /* The device we are working for */
    ide_state = &s->dev[port].port.ifs[0];
    if (!ide_state->blk) {
        DPRINTF(port, "error: guest accessed unused port");
        return -1;
    }

    tbl_addr = le64_to_cpu(cmd->tbl_addr);
    cmd_len = 0x80;
    cmd_fis = dma_memory_map(s->as, tbl_addr, &cmd_len,
                             DMA_DIRECTION_FROM_DEVICE);
    if (!cmd_fis) {
        DPRINTF(port, "error: guest passed us an invalid cmd fis\n");
        return -1;
    } else if (cmd_len != 0x80) {
        ahci_trigger_irq(s, &s->dev[port], PORT_IRQ_HBUS_ERR);
        DPRINTF(port, "error: dma_memory_map failed: "
                "(len(%02"PRIx64") != 0x80)\n",
                cmd_len);
        goto out;
    }
    debug_print_fis(cmd_fis, 0x80);

    switch (cmd_fis[0]) {
        case SATA_FIS_TYPE_REGISTER_H2D:
            handle_reg_h2d_fis(s, port, slot, cmd_fis);
            break;
        default:
            DPRINTF(port, "unknown command cmd_fis[0]=%02x cmd_fis[1]=%02x "
                          "cmd_fis[2]=%02x\n", cmd_fis[0], cmd_fis[1],
                          cmd_fis[2]);
            break;
    }

out:
    dma_memory_unmap(s->as, cmd_fis, cmd_len, DMA_DIRECTION_FROM_DEVICE,
                     cmd_len);

    if (s->dev[port].port.ifs[0].status & (BUSY_STAT|DRQ_STAT)) {
        /* async command, complete later */
        s->dev[port].busy_slot = slot;
        return -1;
    }

    /* done handling the command */
    return 0;
}
