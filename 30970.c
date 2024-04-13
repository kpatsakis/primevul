IsWrongPointerBarrierClient(ClientPtr client, DeviceIntPtr dev, xEvent *event)
{
    xXIBarrierEvent *ev = (xXIBarrierEvent*)event;

    if (ev->type != GenericEvent || ev->extension != IReqCode)
        return FALSE;

    if (ev->evtype != XI_BarrierHit && ev->evtype != XI_BarrierLeave)
        return FALSE;

    return client->index != CLIENT_ID(ev->barrier);
}
