static void bmdma_start_dma(IDEDMA *dma, IDEState *s,
                            BlockCompletionFunc *dma_cb)
{
    BMDMAState *bm = DO_UPCAST(BMDMAState, dma, dma);

    bm->unit = s->unit;
    bm->dma_cb = dma_cb;
    bm->cur_prd_last = 0;
    bm->cur_prd_addr = 0;
    bm->cur_prd_len = 0;
    bm->sector_num = ide_get_sector(s);
    bm->nsector = s->nsector;

    if (bm->status & BM_STATUS_DMAING) {
        bm->dma_cb(bmdma_active_if(bm), 0);
     }
 }
