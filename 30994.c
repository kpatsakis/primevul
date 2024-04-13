ProcUngrabButton(ClientPtr client)
{
    REQUEST(xUngrabButtonReq);
    WindowPtr pWin;
    GrabPtr tempGrab;
    int rc;
    DeviceIntPtr ptr;

    REQUEST_SIZE_MATCH(xUngrabButtonReq);
    UpdateCurrentTime();
    if ((stuff->modifiers != AnyModifier) &&
        (stuff->modifiers & ~AllModifiersMask)) {
        client->errorValue = stuff->modifiers;
        return BadValue;
    }
    rc = dixLookupWindow(&pWin, stuff->grabWindow, client, DixReadAccess);
    if (rc != Success)
        return rc;

    ptr = PickPointer(client);

    tempGrab = AllocGrab(NULL);
    if (!tempGrab)
        return BadAlloc;
    tempGrab->resource = client->clientAsMask;
    tempGrab->device = ptr;
    tempGrab->window = pWin;
    tempGrab->modifiersDetail.exact = stuff->modifiers;
    tempGrab->modifiersDetail.pMask = NULL;
    tempGrab->modifierDevice = GetMaster(ptr, MASTER_KEYBOARD);
    tempGrab->type = ButtonPress;
    tempGrab->detail.exact = stuff->button;
    tempGrab->grabtype = CORE;
    tempGrab->detail.pMask = NULL;
    tempGrab->next = NULL;

    if (!DeletePassiveGrabFromList(tempGrab))
        rc = BadAlloc;

    FreeGrab(tempGrab);
    return rc;
}
