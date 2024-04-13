static int ahci_cond_start_engines(AHCIDevice *ad, bool allow_stop)
{
    AHCIPortRegs *pr = &ad->port_regs;

    if (pr->cmd & PORT_CMD_START) {
        if (ahci_map_clb_address(ad)) {
            pr->cmd |= PORT_CMD_LIST_ON;
        } else {
            error_report("AHCI: Failed to start DMA engine: "
                         "bad command list buffer address");
            return -1;
        }
    } else if (pr->cmd & PORT_CMD_LIST_ON) {
        if (allow_stop) {
            ahci_unmap_clb_address(ad);
            pr->cmd = pr->cmd & ~(PORT_CMD_LIST_ON);
        } else {
            error_report("AHCI: DMA engine should be off, "
                         "but appears to still be running");
            return -1;
        }
    }

    if (pr->cmd & PORT_CMD_FIS_RX) {
        if (ahci_map_fis_address(ad)) {
            pr->cmd |= PORT_CMD_FIS_ON;
        } else {
            error_report("AHCI: Failed to start FIS receive engine: "
                         "bad FIS receive buffer address");
            return -1;
        }
    } else if (pr->cmd & PORT_CMD_FIS_ON) {
        if (allow_stop) {
            ahci_unmap_fis_address(ad);
            pr->cmd = pr->cmd & ~(PORT_CMD_FIS_ON);
        } else {
            error_report("AHCI: FIS receive engine should be off, "
                         "but appears to still be running");
            return -1;
        }
    }

    return 0;
}
