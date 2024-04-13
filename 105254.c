static bool is_request_lun_aligned(int64_t sector_num, int nb_sectors,
                                      IscsiLun *iscsilun)
{
    if ((sector_num * BDRV_SECTOR_SIZE) % iscsilun->block_size ||
        (nb_sectors * BDRV_SECTOR_SIZE) % iscsilun->block_size) {
            error_report("iSCSI misaligned request: "
                         "iscsilun->block_size %u, sector_num %" PRIi64
                         ", nb_sectors %d",
                         iscsilun->block_size, sector_num, nb_sectors);
            return 0;
    }
    return 1;
}
