static int ide_handle_rw_error(IDEState *s, int error, int op)
{
    bool is_read = (op & IDE_RETRY_READ) != 0;
    BlockErrorAction action = blk_get_error_action(s->blk, is_read, error);

    if (action == BLOCK_ERROR_ACTION_STOP) {
        s->bus->dma->ops->set_unit(s->bus->dma, s->unit);
        s->bus->error_status = op;
    } else if (action == BLOCK_ERROR_ACTION_REPORT) {
        if (op & IDE_RETRY_DMA) {
            ide_dma_error(s);
        } else {
            ide_rw_error(s);
        }
    }
    blk_error_action(s->blk, action, is_read, error);
    return action != BLOCK_ERROR_ACTION_IGNORE;
}
