static void ahci_start_dma(IDEDMA *dma, IDEState *s,
                           BlockCompletionFunc *dma_cb)
{
    AHCIDevice *ad = DO_UPCAST(AHCIDevice, dma, dma);
    DPRINTF(ad->port_no, "\n");
    s->io_buffer_offset = 0;
    dma_cb(s, 0);
}
