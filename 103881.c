static int ahci_cond_start_engines(AHCIDevice *ad)
{
    AHCIPortRegs *pr = &ad->port_regs;
    bool cmd_start = pr->cmd & PORT_CMD_START;
    bool cmd_on    = pr->cmd & PORT_CMD_LIST_ON;
    bool fis_start = pr->cmd & PORT_CMD_FIS_RX;
    bool fis_on    = pr->cmd & PORT_CMD_FIS_ON;

    if (cmd_start && !cmd_on) {
        if (!ahci_map_clb_address(ad)) {
            pr->cmd &= ~PORT_CMD_START;
            error_report("AHCI: Failed to start DMA engine: "
                         "bad command list buffer address");
            return -1;
        }
    } else if (!cmd_start && cmd_on) {
        ahci_unmap_clb_address(ad);
    }

    if (fis_start && !fis_on) {
        if (!ahci_map_fis_address(ad)) {
            pr->cmd &= ~PORT_CMD_FIS_RX;
            error_report("AHCI: Failed to start FIS receive engine: "
                         "bad FIS receive buffer address");
            return -1;
        }
    } else if (!fis_start && fis_on) {
        ahci_unmap_fis_address(ad);
    }

    return 0;
}
