static void ahci_unmap_fis_address(AHCIDevice *ad)
{
    dma_memory_unmap(ad->hba->as, ad->res_fis, 256,
                     DMA_DIRECTION_FROM_DEVICE, 256);
    ad->res_fis = NULL;
}
