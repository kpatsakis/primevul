static void execute_ncq_command(NCQTransferState *ncq_tfs)
{
    AHCIDevice *ad = ncq_tfs->drive;
    IDEState *ide_state = &ad->port.ifs[0];
    int port = ad->port_no;

    g_assert(is_ncq(ncq_tfs->cmd));
    ncq_tfs->halt = false;

    switch (ncq_tfs->cmd) {
    case READ_FPDMA_QUEUED:
        DPRINTF(port, "NCQ reading %d sectors from LBA %"PRId64", tag %d\n",
                ncq_tfs->sector_count, ncq_tfs->lba, ncq_tfs->tag);

        DPRINTF(port, "tag %d aio read %"PRId64"\n",
                ncq_tfs->tag, ncq_tfs->lba);

        dma_acct_start(ide_state->blk, &ncq_tfs->acct,
                       &ncq_tfs->sglist, BLOCK_ACCT_READ);
        ncq_tfs->aiocb = dma_blk_read(ide_state->blk, &ncq_tfs->sglist,
                                      ncq_tfs->lba << BDRV_SECTOR_BITS,
                                      BDRV_SECTOR_SIZE,
                                      ncq_cb, ncq_tfs);
        break;
    case WRITE_FPDMA_QUEUED:
        DPRINTF(port, "NCQ writing %d sectors to LBA %"PRId64", tag %d\n",
                ncq_tfs->sector_count, ncq_tfs->lba, ncq_tfs->tag);

        DPRINTF(port, "tag %d aio write %"PRId64"\n",
                ncq_tfs->tag, ncq_tfs->lba);

        dma_acct_start(ide_state->blk, &ncq_tfs->acct,
                       &ncq_tfs->sglist, BLOCK_ACCT_WRITE);
        ncq_tfs->aiocb = dma_blk_write(ide_state->blk, &ncq_tfs->sglist,
                                       ncq_tfs->lba << BDRV_SECTOR_BITS,
                                       BDRV_SECTOR_SIZE,
                                       ncq_cb, ncq_tfs);
        break;
    default:
        DPRINTF(port, "error: unsupported NCQ command (0x%02x) received\n",
                ncq_tfs->cmd);
        ncq_err(ncq_tfs);
    }
}
