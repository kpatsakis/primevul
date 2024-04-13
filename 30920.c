CheckPassiveGrabsOnWindow(WindowPtr pWin,
                          DeviceIntPtr device,
                          InternalEvent *event, BOOL checkCore, BOOL activate)
{
    GrabPtr grab = wPassiveGrabs(pWin);
    GrabPtr tempGrab;

    if (!grab)
        return NULL;

    tempGrab = AllocGrab(NULL);
    if (tempGrab == NULL)
        return NULL;

    /* Fill out the grab details, but leave the type for later before
     * comparing */
    switch (event->any.type) {
    case ET_KeyPress:
    case ET_KeyRelease:
        tempGrab->detail.exact = event->device_event.detail.key;
        break;
    case ET_ButtonPress:
    case ET_ButtonRelease:
    case ET_TouchBegin:
    case ET_TouchEnd:
        tempGrab->detail.exact = event->device_event.detail.button;
        break;
    default:
        tempGrab->detail.exact = 0;
        break;
    }
    tempGrab->window = pWin;
    tempGrab->device = device;
    tempGrab->detail.pMask = NULL;
    tempGrab->modifiersDetail.pMask = NULL;
    tempGrab->next = NULL;

    for (; grab; grab = grab->next) {
        if (!CheckPassiveGrab(device, grab, event, checkCore, tempGrab))
            continue;

        if (activate && !ActivatePassiveGrab(device, grab, event, event))
            continue;

        break;
    }

    FreeGrab(tempGrab);
    return grab;
}
