static bool ahci_map_fis_address(AHCIDevice *ad)
{
    AHCIPortRegs *pr = &ad->port_regs;
    map_page(ad->hba->as, &ad->res_fis,
             ((uint64_t)pr->fis_addr_hi << 32) | pr->fis_addr, 256);
    if (ad->res_fis != NULL) {
        pr->cmd |= PORT_CMD_FIS_ON;
        return true;
    }

    pr->cmd &= ~PORT_CMD_FIS_ON;
    return false;
}
