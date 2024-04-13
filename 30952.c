FixUpEventFromWindow(SpritePtr pSprite,
                     xEvent *xE, WindowPtr pWin, Window child, Bool calcChild)
{
    int evtype;

    if (calcChild)
        child = FindChildForEvent(pSprite, pWin);

    if ((evtype = xi2_get_type(xE))) {
        xXIDeviceEvent *event = (xXIDeviceEvent *) xE;

        switch (evtype) {
        case XI_RawKeyPress:
        case XI_RawKeyRelease:
        case XI_RawButtonPress:
        case XI_RawButtonRelease:
        case XI_RawMotion:
        case XI_RawTouchBegin:
        case XI_RawTouchUpdate:
        case XI_RawTouchEnd:
        case XI_DeviceChanged:
        case XI_HierarchyChanged:
        case XI_PropertyEvent:
        case XI_BarrierHit:
        case XI_BarrierLeave:
            return;
        default:
            break;
        }

        event->root = RootWindow(pSprite)->drawable.id;
        event->event = pWin->drawable.id;

        if (evtype == XI_TouchOwnership) {
            event->child = child;
            return;
        }

        if (pSprite->hot.pScreen == pWin->drawable.pScreen) {
            event->event_x = event->root_x - double_to_fp1616(pWin->drawable.x);
            event->event_y = event->root_y - double_to_fp1616(pWin->drawable.y);
            event->child = child;
        }
        else {
            event->event_x = 0;
            event->event_y = 0;
            event->child = None;
        }

        if (event->evtype == XI_Enter || event->evtype == XI_Leave ||
            event->evtype == XI_FocusIn || event->evtype == XI_FocusOut)
            ((xXIEnterEvent *) event)->same_screen =
                (pSprite->hot.pScreen == pWin->drawable.pScreen);

    }
    else {
        XE_KBPTR.root = RootWindow(pSprite)->drawable.id;
        XE_KBPTR.event = pWin->drawable.id;
        if (pSprite->hot.pScreen == pWin->drawable.pScreen) {
            XE_KBPTR.sameScreen = xTrue;
            XE_KBPTR.child = child;
            XE_KBPTR.eventX = XE_KBPTR.rootX - pWin->drawable.x;
            XE_KBPTR.eventY = XE_KBPTR.rootY - pWin->drawable.y;
        }
        else {
            XE_KBPTR.sameScreen = xFalse;
            XE_KBPTR.child = None;
            XE_KBPTR.eventX = 0;
            XE_KBPTR.eventY = 0;
        }
    }
}
