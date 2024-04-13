XICreatePointerBarrier(ClientPtr client,
                       xXFixesCreatePointerBarrierReq * stuff)
{
    int err;
    struct PointerBarrierClient *barrier;
    struct PointerBarrier b;

    b.x1 = stuff->x1;
    b.x2 = stuff->x2;
    b.y1 = stuff->y1;
    b.y2 = stuff->y2;

    if (!barrier_is_horizontal(&b) && !barrier_is_vertical(&b))
        return BadValue;

    /* no 0-sized barriers */
    if (barrier_is_horizontal(&b) && barrier_is_vertical(&b))
        return BadValue;

    /* no infinite barriers on the wrong axis */
    if (barrier_is_horizontal(&b) && (b.y1 < 0 || b.y2 < 0))
        return BadValue;

    if (barrier_is_vertical(&b) && (b.x1 < 0 || b.x2 < 0))
        return BadValue;

    if ((err = CreatePointerBarrierClient(client, stuff, &barrier)))
        return err;

    if (!AddResource(stuff->barrier, PointerBarrierType, &barrier->barrier))
        return BadAlloc;

    return Success;
}
