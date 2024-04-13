coroutine_fn iscsi_co_discard(BlockDriverState *bs, int64_t sector_num,
                                   int nb_sectors)
{
    IscsiLun *iscsilun = bs->opaque;
    struct IscsiTask iTask;
    struct unmap_list list;

    if (!is_request_lun_aligned(sector_num, nb_sectors, iscsilun)) {
        return -EINVAL;
    }

    if (!iscsilun->lbp.lbpu) {
        /* UNMAP is not supported by the target */
        return 0;
    }

    list.lba = sector_qemu2lun(sector_num, iscsilun);
    list.num = sector_qemu2lun(nb_sectors, iscsilun);

    iscsi_co_init_iscsitask(iscsilun, &iTask);
retry:
    if (iscsi_unmap_task(iscsilun->iscsi, iscsilun->lun, 0, 0, &list, 1,
                     iscsi_co_generic_cb, &iTask) == NULL) {
        return -ENOMEM;
    }

    while (!iTask.complete) {
        iscsi_set_events(iscsilun);
        qemu_coroutine_yield();
    }

    if (iTask.task != NULL) {
        scsi_free_scsi_task(iTask.task);
        iTask.task = NULL;
    }

    if (iTask.do_retry) {
        iTask.complete = 0;
        goto retry;
    }

    if (iTask.status == SCSI_STATUS_CHECK_CONDITION) {
        /* the target might fail with a check condition if it
           is not happy with the alignment of the UNMAP request
           we silently fail in this case */
        return 0;
    }

    if (iTask.status != SCSI_STATUS_GOOD) {
        return iTask.err_code;
    }

    iscsi_allocationmap_clear(iscsilun, sector_num, nb_sectors);

    return 0;
}
