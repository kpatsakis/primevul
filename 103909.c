static void ahci_unmap_fis_address(AHCIDevice *ad)
{
    if (ad->res_fis == NULL) {
        DPRINTF(ad->port_no, "Attempt to unmap NULL FIS address\n");
        return;
    }
    ad->port_regs.cmd &= ~PORT_CMD_FIS_ON;
    dma_memory_unmap(ad->hba->as, ad->res_fis, 256,
                     DMA_DIRECTION_FROM_DEVICE, 256);
    ad->res_fis = NULL;
}
