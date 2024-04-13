static void ncq_cb(void *opaque, int ret)
{
    NCQTransferState *ncq_tfs = (NCQTransferState *)opaque;
    IDEState *ide_state = &ncq_tfs->drive->port.ifs[0];

    ncq_tfs->aiocb = NULL;
    if (ret == -ECANCELED) {
        return;
    }

    if (ret < 0) {
        bool is_read = ncq_tfs->cmd == READ_FPDMA_QUEUED;
        BlockErrorAction action = blk_get_error_action(ide_state->blk,
                                                       is_read, -ret);
        if (action == BLOCK_ERROR_ACTION_STOP) {
            ncq_tfs->halt = true;
            ide_state->bus->error_status = IDE_RETRY_HBA;
        } else if (action == BLOCK_ERROR_ACTION_REPORT) {
            ncq_err(ncq_tfs);
        }
        blk_error_action(ide_state->blk, action, is_read, -ret);
    } else {
        ide_state->status = READY_STAT | SEEK_STAT;
    }

    if (!ncq_tfs->halt) {
        ncq_finish(ncq_tfs);
    }
}
