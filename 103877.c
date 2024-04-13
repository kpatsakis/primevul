static void ahci_check_cmd_bh(void *opaque)
{
    AHCIDevice *ad = opaque;

    qemu_bh_delete(ad->check_bh);
    ad->check_bh = NULL;

    if ((ad->busy_slot != -1) &&
        !(ad->port.ifs[0].status & (BUSY_STAT|DRQ_STAT))) {
        /* no longer busy */
        ad->port_regs.cmd_issue &= ~(1 << ad->busy_slot);
        ad->busy_slot = -1;
    }

    check_cmd(ad->hba, ad->port_no);
}
