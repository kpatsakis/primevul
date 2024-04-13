static bool ahci_map_clb_address(AHCIDevice *ad)
{
    AHCIPortRegs *pr = &ad->port_regs;
    ad->cur_cmd = NULL;
    map_page(ad->hba->as, &ad->lst,
             ((uint64_t)pr->lst_addr_hi << 32) | pr->lst_addr, 1024);
    if (ad->lst != NULL) {
        pr->cmd |= PORT_CMD_LIST_ON;
        return true;
    }

    pr->cmd &= ~PORT_CMD_LIST_ON;
    return false;
}
