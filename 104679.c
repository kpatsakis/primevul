pvscsi_schedule_completion_processing(PVSCSIState *s)
{
    /* Try putting more complete requests on the ring. */
    if (!QTAILQ_EMPTY(&s->completion_queue)) {
        qemu_bh_schedule(s->completion_worker);
    }
}
