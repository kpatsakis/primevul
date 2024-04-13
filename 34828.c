static void remove_master_func(void *res, XID id, void *devid)
{
    struct PointerBarrierDevice *pbd;
    struct PointerBarrierClient *barrier;
    struct PointerBarrier *b;
    DeviceIntPtr dev;
    int *deviceid = devid;
    int rc;
    Time ms = GetTimeInMillis();

    rc = dixLookupDevice(&dev, *deviceid, serverClient, DixSendAccess);
    if (rc != Success)
        return;

    b = res;
    barrier = container_of(b, struct PointerBarrierClient, barrier);

    pbd = GetBarrierDevice(barrier, *deviceid);

    if (pbd->hit) {
        BarrierEvent ev = {
            .header = ET_Internal,
            .type =ET_BarrierLeave,
            .length = sizeof (BarrierEvent),
            .time = ms,
            .deviceid = *deviceid,
            .sourceid = 0,
            .dx = 0,
            .dy = 0,
            .root = barrier->screen->root->drawable.id,
            .window = barrier->window,
            .dt = ms - pbd->last_timestamp,
            .flags = XIBarrierPointerReleased,
            .event_id = pbd->barrier_event_id,
            .barrierid = barrier->id,
        };

        mieqEnqueue(dev, (InternalEvent *) &ev);
    }

    xorg_list_del(&pbd->entry);
    free(pbd);
}
