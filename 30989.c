ProcGrabKey(ClientPtr client)
{
    WindowPtr pWin;

    REQUEST(xGrabKeyReq);
    GrabPtr grab;
    DeviceIntPtr keybd = PickKeyboard(client);
    int rc;
    GrabParameters param;
    GrabMask mask;

    REQUEST_SIZE_MATCH(xGrabKeyReq);

    param = (GrabParameters) {
        .grabtype = CORE,
        .ownerEvents = stuff->ownerEvents,
        .this_device_mode = stuff->keyboardMode,
        .other_devices_mode = stuff->pointerMode,
        .modifiers = stuff->modifiers
    };

    rc = CheckGrabValues(client, &param);
    if (rc != Success)
        return rc;

    if (((stuff->key > keybd->key->xkbInfo->desc->max_key_code) ||
         (stuff->key < keybd->key->xkbInfo->desc->min_key_code))
        && (stuff->key != AnyKey)) {
        client->errorValue = stuff->key;
        return BadValue;
    }
    rc = dixLookupWindow(&pWin, stuff->grabWindow, client, DixSetAttrAccess);
    if (rc != Success)
        return rc;

    mask.core = (KeyPressMask | KeyReleaseMask);

    grab = CreateGrab(client->index, keybd, keybd, pWin, CORE, &mask,
                      &param, KeyPress, stuff->key, NullWindow, NullCursor);
    if (!grab)
        return BadAlloc;
    return AddPassiveGrabToList(client, grab);
}
