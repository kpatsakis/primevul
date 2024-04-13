CoreEnterLeaveEvent(DeviceIntPtr mouse,
                    int type,
                    int mode, int detail, WindowPtr pWin, Window child)
{
    xEvent event = {
        .u.u.type = type,
        .u.u.detail = detail
    };
    WindowPtr focus;
    DeviceIntPtr keybd;
    GrabPtr grab = mouse->deviceGrab.grab;
    Mask mask;

    keybd = GetMaster(mouse, KEYBOARD_OR_FLOAT);

    if ((pWin == mouse->valuator->motionHintWindow) &&
        (detail != NotifyInferior))
        mouse->valuator->motionHintWindow = NullWindow;
    if (grab) {
        mask = (pWin == grab->window) ? grab->eventMask : 0;
        if (grab->ownerEvents)
            mask |= EventMaskForClient(pWin, rClient(grab));
    }
    else {
        mask = pWin->eventMask | wOtherEventMasks(pWin);
    }

    event.u.enterLeave.time = currentTime.milliseconds;
    event.u.enterLeave.rootX = mouse->spriteInfo->sprite->hot.x;
    event.u.enterLeave.rootY = mouse->spriteInfo->sprite->hot.y;
    /* Counts on the same initial structure of crossing & button events! */
    FixUpEventFromWindow(mouse->spriteInfo->sprite, &event, pWin, None, FALSE);
    /* Enter/Leave events always set child */
    event.u.enterLeave.child = child;
    event.u.enterLeave.flags = event.u.keyButtonPointer.sameScreen ?
        ELFlagSameScreen : 0;
    event.u.enterLeave.state =
        mouse->button ? (mouse->button->state & 0x1f00) : 0;
    if (keybd)
        event.u.enterLeave.state |=
            XkbGrabStateFromRec(&keybd->key->xkbInfo->state);
    event.u.enterLeave.mode = mode;
    focus = (keybd) ? keybd->focus->win : None;
    if ((focus != NoneWin) &&
        ((pWin == focus) || (focus == PointerRootWin) || IsParent(focus, pWin)))
        event.u.enterLeave.flags |= ELFlagFocus;

    if ((mask & GetEventFilter(mouse, &event))) {
        if (grab)
            TryClientEvents(rClient(grab), mouse, &event, 1, mask,
                            GetEventFilter(mouse, &event), grab);
        else
            DeliverEventsToWindow(mouse, pWin, &event, 1,
                                  GetEventFilter(mouse, &event), NullGrab);
    }

    if ((type == EnterNotify) && (mask & KeymapStateMask)) {
        xKeymapEvent ke = {
            .type = KeymapNotify
        };
        ClientPtr client = grab ? rClient(grab) : wClient(pWin);
        int rc;

        rc = XaceHook(XACE_DEVICE_ACCESS, client, keybd, DixReadAccess);
        if (rc == Success)
            memcpy((char *) &ke.map[0], (char *) &keybd->key->down[1], 31);

        if (grab)
            TryClientEvents(rClient(grab), keybd, (xEvent *) &ke, 1,
                            mask, KeymapStateMask, grab);
        else
            DeliverEventsToWindow(mouse, pWin, (xEvent *) &ke, 1,
                                  KeymapStateMask, NullGrab);
    }
}
