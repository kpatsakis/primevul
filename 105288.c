static void iscsi_refresh_limits(BlockDriverState *bs, Error **errp)
{
    /* We don't actually refresh here, but just return data queried in
     * iscsi_open(): iscsi targets don't change their limits. */

    IscsiLun *iscsilun = bs->opaque;
    uint32_t max_xfer_len = iscsilun->use_16_for_rw ? 0xffffffff : 0xffff;

    if (iscsilun->bl.max_xfer_len) {
        max_xfer_len = MIN(max_xfer_len, iscsilun->bl.max_xfer_len);
    }

    bs->bl.max_transfer_length = sector_limits_lun2qemu(max_xfer_len, iscsilun);

    if (iscsilun->lbp.lbpu) {
        if (iscsilun->bl.max_unmap < 0xffffffff) {
            bs->bl.max_discard =
                sector_limits_lun2qemu(iscsilun->bl.max_unmap, iscsilun);
        }
        bs->bl.discard_alignment =
            sector_limits_lun2qemu(iscsilun->bl.opt_unmap_gran, iscsilun);
    }

    if (iscsilun->bl.max_ws_len < 0xffffffff) {
        bs->bl.max_write_zeroes =
            sector_limits_lun2qemu(iscsilun->bl.max_ws_len, iscsilun);
    }
    if (iscsilun->lbp.lbpws) {
        bs->bl.write_zeroes_alignment =
            sector_limits_lun2qemu(iscsilun->bl.opt_unmap_gran, iscsilun);
    }
    bs->bl.opt_transfer_length =
        sector_limits_lun2qemu(iscsilun->bl.opt_xfer_len, iscsilun);
}
