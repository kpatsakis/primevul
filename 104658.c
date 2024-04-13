pvscsi_pre_save(void *opaque)
{
    PVSCSIState *s = (PVSCSIState *) opaque;

    trace_pvscsi_state("presave");

    assert(QTAILQ_EMPTY(&s->pending_queue));
    assert(QTAILQ_EMPTY(&s->completion_queue));
}
