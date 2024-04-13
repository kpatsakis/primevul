static int iscsi_get_info(BlockDriverState *bs, BlockDriverInfo *bdi)
{
    IscsiLun *iscsilun = bs->opaque;
    bdi->unallocated_blocks_are_zero = iscsilun->lbprz;
    bdi->can_write_zeroes_with_unmap = iscsilun->lbprz && iscsilun->lbp.lbpws;
    bdi->cluster_size = iscsilun->cluster_sectors * BDRV_SECTOR_SIZE;
    return 0;
}
