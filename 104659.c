pvscsi_process_completion_queue(void *opaque)
{
    PVSCSIState *s = opaque;
    PVSCSIRequest *pvscsi_req;
    bool has_completed = false;

    while (!QTAILQ_EMPTY(&s->completion_queue)) {
        pvscsi_req = QTAILQ_FIRST(&s->completion_queue);
        QTAILQ_REMOVE(&s->completion_queue, pvscsi_req, next);
        pvscsi_cmp_ring_put(s, &pvscsi_req->cmp);
        g_free(pvscsi_req);
        has_completed = true;
    }

    if (has_completed) {
        pvscsi_ring_flush_cmp(&s->rings);
        pvscsi_raise_completion_interrupt(s);
    }
}
