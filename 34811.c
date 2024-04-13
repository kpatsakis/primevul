BarrierFreeBarrier(void *data, XID id)
{
    struct PointerBarrierClient *c;
    Time ms = GetTimeInMillis();
    DeviceIntPtr dev = NULL;
    ScreenPtr screen;

    c = container_of(data, struct PointerBarrierClient, barrier);
    screen = c->screen;

    for (dev = inputInfo.devices; dev; dev = dev->next) {
        struct PointerBarrierDevice *pbd;
        int root_x, root_y;
        BarrierEvent ev = {
            .header = ET_Internal,
            .type = ET_BarrierLeave,
            .length = sizeof (BarrierEvent),
            .time = ms,
            /* .deviceid */
            .sourceid = 0,
            .barrierid = c->id,
            .window = c->window,
            .root = screen->root->drawable.id,
            .dx = 0,
            .dy = 0,
            /* .root_x */
            /* .root_y */
            /* .dt */
            /* .event_id */
            .flags = XIBarrierPointerReleased,
        };


        if (dev->type != MASTER_POINTER)
            continue;

        pbd = GetBarrierDevice(c, dev->id);
        if (!pbd->hit)
            continue;

        ev.deviceid = dev->id;
        ev.event_id = pbd->barrier_event_id;
        ev.dt = ms - pbd->last_timestamp;

        GetSpritePosition(dev, &root_x, &root_y);
        ev.root_x = root_x;
        ev.root_y = root_y;

        mieqEnqueue(dev, (InternalEvent *) &ev);
    }

    xorg_list_del(&c->entry);

    FreePointerBarrierClient(c);
    return Success;
}
