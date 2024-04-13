DeliverGrabbedEvent(InternalEvent *event, DeviceIntPtr thisDev,
                    Bool deactivateGrab)
{
    GrabPtr grab;
    GrabInfoPtr grabinfo;
    int deliveries = 0;
    DeviceIntPtr dev;
    SpritePtr pSprite = thisDev->spriteInfo->sprite;
    BOOL sendCore = FALSE;

    grabinfo = &thisDev->deviceGrab;
    grab = grabinfo->grab;

    if (grab->ownerEvents) {
        WindowPtr focus;

        /* Hack: Some pointer device have a focus class. So we need to check
         * for the type of event, to see if we really want to deliver it to
         * the focus window. For pointer events, the answer is no.
         */
        if (IsPointerEvent(event))
            focus = PointerRootWin;
        else if (thisDev->focus) {
            focus = thisDev->focus->win;
            if (focus == FollowKeyboardWin)
                focus = inputInfo.keyboard->focus->win;
        }
        else
            focus = PointerRootWin;
        if (focus == PointerRootWin)
            deliveries = DeliverDeviceEvents(pSprite->win, event, grab,
                                             NullWindow, thisDev);
        else if (focus && (focus == pSprite->win ||
                           IsParent(focus, pSprite->win)))
            deliveries = DeliverDeviceEvents(pSprite->win, event, grab, focus,
                                             thisDev);
        else if (focus)
            deliveries = DeliverDeviceEvents(focus, event, grab, focus,
                                             thisDev);
    }
    if (!deliveries) {
        sendCore = (IsMaster(thisDev) && thisDev->coreEvents);
        /* try core event */
        if ((sendCore && grab->grabtype == CORE) || grab->grabtype != CORE)
            deliveries = DeliverOneGrabbedEvent(event, thisDev, grab->grabtype);

        if (deliveries && (event->any.type == ET_Motion))
            thisDev->valuator->motionHintWindow = grab->window;
    }
    if (deliveries && !deactivateGrab &&
        (event->any.type == ET_KeyPress ||
         event->any.type == ET_KeyRelease ||
         event->any.type == ET_ButtonPress ||
         event->any.type == ET_ButtonRelease)) {
        switch (grabinfo->sync.state) {
        case FREEZE_BOTH_NEXT_EVENT:
            dev = GetPairedDevice(thisDev);
            if (dev) {
                FreezeThaw(dev, TRUE);
                if ((dev->deviceGrab.sync.state == FREEZE_BOTH_NEXT_EVENT) &&
                    (CLIENT_BITS(grab->resource) ==
                     CLIENT_BITS(dev->deviceGrab.grab->resource)))
                    dev->deviceGrab.sync.state = FROZEN_NO_EVENT;
                else
                    dev->deviceGrab.sync.other = grab;
            }
            /* fall through */
        case FREEZE_NEXT_EVENT:
            grabinfo->sync.state = FROZEN_WITH_EVENT;
            FreezeThaw(thisDev, TRUE);
            *grabinfo->sync.event = event->device_event;
            break;
        }
    }

    return deliveries;
}
