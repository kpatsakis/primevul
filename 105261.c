static void iscsi_allocationmap_set(IscsiLun *iscsilun, int64_t sector_num,
                                    int nb_sectors)
{
    if (iscsilun->allocationmap == NULL) {
        return;
    }
    bitmap_set(iscsilun->allocationmap,
               sector_num / iscsilun->cluster_sectors,
               DIV_ROUND_UP(nb_sectors, iscsilun->cluster_sectors));
}
