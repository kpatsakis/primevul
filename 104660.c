pvscsi_process_request_descriptor(PVSCSIState *s,
                                  struct PVSCSIRingReqDesc *descr)
{
    SCSIDevice *d;
    PVSCSIRequest *r = pvscsi_queue_pending_descriptor(s, &d, descr);
    int64_t n;

    trace_pvscsi_process_req_descr(descr->cdb[0], descr->context);

    if (!d) {
        r->cmp.hostStatus = BTSTAT_SELTIMEO;
        trace_pvscsi_process_req_descr_unknown_device();
        pvscsi_complete_request(s, r);
        return;
    }

    if (descr->flags & PVSCSI_FLAG_CMD_WITH_SG_LIST) {
        r->sg.elemAddr = descr->dataAddr;
    }

    r->sreq = scsi_req_new(d, descr->context, r->lun, descr->cdb, r);
    if (r->sreq->cmd.mode == SCSI_XFER_FROM_DEV &&
        (descr->flags & PVSCSI_FLAG_CMD_DIR_TODEVICE)) {
        r->cmp.hostStatus = BTSTAT_BADMSG;
        trace_pvscsi_process_req_descr_invalid_dir();
        scsi_req_cancel(r->sreq);
        return;
    }
    if (r->sreq->cmd.mode == SCSI_XFER_TO_DEV &&
        (descr->flags & PVSCSI_FLAG_CMD_DIR_TOHOST)) {
        r->cmp.hostStatus = BTSTAT_BADMSG;
        trace_pvscsi_process_req_descr_invalid_dir();
        scsi_req_cancel(r->sreq);
        return;
    }

    pvscsi_build_sglist(s, r);
    n = scsi_req_enqueue(r->sreq);

    if (n) {
        scsi_req_continue(r->sreq);
    }
}
