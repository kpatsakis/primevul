void qcow2_process_discards(BlockDriverState *bs, int ret)
{
    BDRVQcowState *s = bs->opaque;
    Qcow2DiscardRegion *d, *next;

    QTAILQ_FOREACH_SAFE(d, &s->discards, next, next) {
        QTAILQ_REMOVE(&s->discards, d, next);

        /* Discard is optional, ignore the return value */
        if (ret >= 0) {
            bdrv_discard(bs->file,
                         d->offset >> BDRV_SECTOR_BITS,
                         d->bytes >> BDRV_SECTOR_BITS);
        }

        g_free(d);
    }
}
