static void ahci_restart_dma(IDEDMA *dma)
{
    /* Nothing to do, ahci_start_dma already resets s->io_buffer_offset.  */
}
