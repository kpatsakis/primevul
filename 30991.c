ProcGrabPointer(ClientPtr client)
{
    xGrabPointerReply rep;
    DeviceIntPtr device = PickPointer(client);
    GrabPtr grab;
    GrabMask mask;
    WindowPtr confineTo;
    BYTE status;

    REQUEST(xGrabPointerReq);
    int rc;

    REQUEST_SIZE_MATCH(xGrabPointerReq);
    UpdateCurrentTime();

    if (stuff->eventMask & ~PointerGrabMask) {
        client->errorValue = stuff->eventMask;
        return BadValue;
    }

    if (stuff->confineTo == None)
        confineTo = NullWindow;
    else {
        rc = dixLookupWindow(&confineTo, stuff->confineTo, client,
                             DixSetAttrAccess);
        if (rc != Success)
            return rc;
    }

    grab = device->deviceGrab.grab;

    if (grab && grab->confineTo && !confineTo)
        ConfineCursorToWindow(device, GetCurrentRootWindow(device), FALSE, FALSE);

    mask.core = stuff->eventMask;

    rc = GrabDevice(client, device, stuff->pointerMode, stuff->keyboardMode,
                    stuff->grabWindow, stuff->ownerEvents, stuff->time,
                    &mask, CORE, stuff->cursor, stuff->confineTo, &status);
    if (rc != Success)
        return rc;

    rep = (xGrabPointerReply) {
        .type = X_Reply,
        .status = status,
        .sequenceNumber = client->sequence,
        .length = 0
    };
    WriteReplyToClient(client, sizeof(xGrabPointerReply), &rep);
    return Success;
}
