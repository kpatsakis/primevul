static void ahci_restart(IDEDMA *dma)
{
    AHCIDevice *ad = DO_UPCAST(AHCIDevice, dma, dma);
    int i;

    for (i = 0; i < AHCI_MAX_CMDS; i++) {
        NCQTransferState *ncq_tfs = &ad->ncq_tfs[i];
        if (ncq_tfs->halt) {
            execute_ncq_command(ncq_tfs);
        }
    }
}
