GrabDevice(ClientPtr client, DeviceIntPtr dev,
           unsigned pointer_mode, unsigned keyboard_mode, Window grabWindow,
           unsigned ownerEvents, Time ctime, GrabMask *mask,
           int grabtype, Cursor curs, Window confineToWin, CARD8 *status)
{
    WindowPtr pWin, confineTo;
    GrabPtr grab;
    TimeStamp time;
    Mask access_mode = DixGrabAccess;
    int rc;
    GrabInfoPtr grabInfo = &dev->deviceGrab;
    CursorPtr cursor;

    UpdateCurrentTime();
    if ((keyboard_mode != GrabModeSync) && (keyboard_mode != GrabModeAsync)) {
        client->errorValue = keyboard_mode;
        return BadValue;
    }
    if ((pointer_mode != GrabModeSync) && (pointer_mode != GrabModeAsync)) {
        client->errorValue = pointer_mode;
        return BadValue;
    }
    if ((ownerEvents != xFalse) && (ownerEvents != xTrue)) {
        client->errorValue = ownerEvents;
        return BadValue;
    }

    rc = dixLookupWindow(&pWin, grabWindow, client, DixSetAttrAccess);
    if (rc != Success)
        return rc;

    if (confineToWin == None)
        confineTo = NullWindow;
    else {
        rc = dixLookupWindow(&confineTo, confineToWin, client,
                             DixSetAttrAccess);
        if (rc != Success)
            return rc;
    }

    if (curs == None)
        cursor = NullCursor;
    else {
        rc = dixLookupResourceByType((void **) &cursor, curs, RT_CURSOR,
                                     client, DixUseAccess);
        if (rc != Success) {
            client->errorValue = curs;
            return rc;
        }
        access_mode |= DixForceAccess;
    }

    if (keyboard_mode == GrabModeSync || pointer_mode == GrabModeSync)
        access_mode |= DixFreezeAccess;
    rc = XaceHook(XACE_DEVICE_ACCESS, client, dev, access_mode);
    if (rc != Success)
        return rc;

    time = ClientTimeToServerTime(ctime);
    grab = grabInfo->grab;
    if (grab && grab->grabtype != grabtype)
        *status = AlreadyGrabbed;
    else if (grab && !SameClient(grab, client))
        *status = AlreadyGrabbed;
    else if ((!pWin->realized) ||
             (confineTo &&
              !(confineTo->realized && BorderSizeNotEmpty(dev, confineTo))))
        *status = GrabNotViewable;
    else if ((CompareTimeStamps(time, currentTime) == LATER) ||
             (CompareTimeStamps(time, grabInfo->grabTime) == EARLIER))
        *status = GrabInvalidTime;
    else if (grabInfo->sync.frozen &&
             grabInfo->sync.other && !SameClient(grabInfo->sync.other, client))
        *status = GrabFrozen;
    else {
        GrabPtr tempGrab;

        tempGrab = AllocGrab(NULL);
        if (tempGrab == NULL)
            return BadAlloc;

        tempGrab->next = NULL;
        tempGrab->window = pWin;
        tempGrab->resource = client->clientAsMask;
        tempGrab->ownerEvents = ownerEvents;
        tempGrab->keyboardMode = keyboard_mode;
        tempGrab->pointerMode = pointer_mode;
        if (grabtype == CORE)
            tempGrab->eventMask = mask->core;
        else if (grabtype == XI)
            tempGrab->eventMask = mask->xi;
        else
            xi2mask_merge(tempGrab->xi2mask, mask->xi2mask);
        tempGrab->device = dev;
        tempGrab->cursor = RefCursor(cursor);
        tempGrab->confineTo = confineTo;
        tempGrab->grabtype = grabtype;
        (*grabInfo->ActivateGrab) (dev, tempGrab, time, FALSE);
        *status = GrabSuccess;

        FreeGrab(tempGrab);
    }
    return Success;
}
