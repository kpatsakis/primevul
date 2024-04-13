pvscsi_reset_adapter(PVSCSIState *s)
{
    s->resetting++;
    qbus_reset_all_fn(&s->bus);
    s->resetting--;
    pvscsi_process_completion_queue(s);
    assert(QTAILQ_EMPTY(&s->pending_queue));
    pvscsi_reset_state(s);
}
