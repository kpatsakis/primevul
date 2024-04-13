ActivateImplicitGrab(DeviceIntPtr dev, ClientPtr client, WindowPtr win,
                     xEvent *event, Mask deliveryMask)
{
    GrabPtr tempGrab;
    OtherInputMasks *inputMasks;
    CARD8 type = event->u.u.type;
    enum InputLevel grabtype;

    if (type == ButtonPress)
        grabtype = CORE;
    else if (type == DeviceButtonPress)
        grabtype = XI;
    else if ((type = xi2_get_type(event)) == XI_ButtonPress)
        grabtype = XI2;
    else
        return FALSE;

    tempGrab = AllocGrab(NULL);
    if (!tempGrab)
        return FALSE;
    tempGrab->next = NULL;
    tempGrab->device = dev;
    tempGrab->resource = client->clientAsMask;
    tempGrab->window = win;
    tempGrab->ownerEvents = (deliveryMask & OwnerGrabButtonMask) ? TRUE : FALSE;
    tempGrab->eventMask = deliveryMask;
    tempGrab->keyboardMode = GrabModeAsync;
    tempGrab->pointerMode = GrabModeAsync;
    tempGrab->confineTo = NullWindow;
    tempGrab->cursor = NullCursor;
    tempGrab->type = type;
    tempGrab->grabtype = grabtype;

    /* get the XI and XI2 device mask */
    inputMasks = wOtherInputMasks(win);
    tempGrab->deviceMask = (inputMasks) ? inputMasks->inputEvents[dev->id] : 0;

    if (inputMasks)
        xi2mask_merge(tempGrab->xi2mask, inputMasks->xi2mask);

    (*dev->deviceGrab.ActivateGrab) (dev, tempGrab,
                                     currentTime, TRUE | ImplicitGrabMask);
    FreeGrab(tempGrab);
    return TRUE;
}
