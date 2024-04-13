static void ahci_unmap_clb_address(AHCIDevice *ad)
{
    dma_memory_unmap(ad->hba->as, ad->lst, 1024,
                     DMA_DIRECTION_FROM_DEVICE, 1024);
    ad->lst = NULL;
}
