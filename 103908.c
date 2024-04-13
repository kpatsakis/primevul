static void ahci_unmap_clb_address(AHCIDevice *ad)
{
    if (ad->lst == NULL) {
        DPRINTF(ad->port_no, "Attempt to unmap NULL CLB address\n");
        return;
    }
    ad->port_regs.cmd &= ~PORT_CMD_LIST_ON;
    dma_memory_unmap(ad->hba->as, ad->lst, 1024,
                     DMA_DIRECTION_FROM_DEVICE, 1024);
    ad->lst = NULL;
}
