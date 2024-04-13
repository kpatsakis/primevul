pvscsi_complete_request(PVSCSIState *s, PVSCSIRequest *r)
{
    assert(!r->completed);

    trace_pvscsi_complete_request(r->cmp.context, r->cmp.dataLen,
                                  r->sense_key);
    if (r->sreq != NULL) {
        scsi_req_unref(r->sreq);
        r->sreq = NULL;
    }
    r->completed = 1;
    QTAILQ_REMOVE(&s->pending_queue, r, next);
    QTAILQ_INSERT_TAIL(&s->completion_queue, r, next);
    pvscsi_schedule_completion_processing(s);
}
