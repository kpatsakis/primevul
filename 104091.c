void ide_flush_cache(IDEState *s)
{
    if (s->blk == NULL) {
        ide_flush_cb(s, 0);
        return;
    }

    s->status |= BUSY_STAT;
    block_acct_start(blk_get_stats(s->blk), &s->acct, 0, BLOCK_ACCT_FLUSH);
    s->pio_aiocb = blk_aio_flush(s->blk, ide_flush_cb, s);
}
