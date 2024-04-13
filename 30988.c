ProcGrabButton(ClientPtr client)
{
    WindowPtr pWin, confineTo;

    REQUEST(xGrabButtonReq);
    CursorPtr cursor;
    GrabPtr grab;
    DeviceIntPtr ptr, modifierDevice;
    Mask access_mode = DixGrabAccess;
    GrabMask mask;
    GrabParameters param;
    int rc;

    REQUEST_SIZE_MATCH(xGrabButtonReq);
    UpdateCurrentTime();
    if ((stuff->pointerMode != GrabModeSync) &&
        (stuff->pointerMode != GrabModeAsync)) {
        client->errorValue = stuff->pointerMode;
        return BadValue;
    }
    if ((stuff->keyboardMode != GrabModeSync) &&
        (stuff->keyboardMode != GrabModeAsync)) {
        client->errorValue = stuff->keyboardMode;
        return BadValue;
    }
    if ((stuff->modifiers != AnyModifier) &&
        (stuff->modifiers & ~AllModifiersMask)) {
        client->errorValue = stuff->modifiers;
        return BadValue;
    }
    if ((stuff->ownerEvents != xFalse) && (stuff->ownerEvents != xTrue)) {
        client->errorValue = stuff->ownerEvents;
        return BadValue;
    }
    if (stuff->eventMask & ~PointerGrabMask) {
        client->errorValue = stuff->eventMask;
        return BadValue;
    }
    rc = dixLookupWindow(&pWin, stuff->grabWindow, client, DixSetAttrAccess);
    if (rc != Success)
        return rc;
    if (stuff->confineTo == None)
        confineTo = NullWindow;
    else {
        rc = dixLookupWindow(&confineTo, stuff->confineTo, client,
                             DixSetAttrAccess);
        if (rc != Success)
            return rc;
    }
    if (stuff->cursor == None)
        cursor = NullCursor;
    else {
        rc = dixLookupResourceByType((void **) &cursor, stuff->cursor,
                                     RT_CURSOR, client, DixUseAccess);
        if (rc != Success) {
            client->errorValue = stuff->cursor;
            return rc;
        }
        access_mode |= DixForceAccess;
    }

    ptr = PickPointer(client);
    modifierDevice = GetMaster(ptr, MASTER_KEYBOARD);
    if (stuff->pointerMode == GrabModeSync ||
        stuff->keyboardMode == GrabModeSync)
        access_mode |= DixFreezeAccess;
    rc = XaceHook(XACE_DEVICE_ACCESS, client, ptr, access_mode);
    if (rc != Success)
        return rc;

    param = (GrabParameters) {
        .grabtype = CORE,
        .ownerEvents = stuff->ownerEvents,
        .this_device_mode = stuff->keyboardMode,
        .other_devices_mode = stuff->pointerMode,
        .modifiers = stuff->modifiers
    };

    mask.core = stuff->eventMask;

    grab = CreateGrab(client->index, ptr, modifierDevice, pWin,
                      CORE, &mask, &param, ButtonPress,
                      stuff->button, confineTo, cursor);
    if (!grab)
        return BadAlloc;
    return AddPassiveGrabToList(client, grab);
}
