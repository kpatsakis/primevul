DeliverFocusedEvent(DeviceIntPtr keybd, InternalEvent *event, WindowPtr window)
{
    DeviceIntPtr ptr;
    WindowPtr focus = keybd->focus->win;
    BOOL sendCore = (IsMaster(keybd) && keybd->coreEvents);
    xEvent *core = NULL, *xE = NULL, *xi2 = NULL;
    int count, rc;
    int deliveries = 0;

    if (focus == FollowKeyboardWin)
        focus = inputInfo.keyboard->focus->win;
    if (!focus)
        return;
    if (focus == PointerRootWin) {
        DeliverDeviceEvents(window, event, NullGrab, NullWindow, keybd);
        return;
    }
    if ((focus == window) || IsParent(focus, window)) {
        if (DeliverDeviceEvents(window, event, NullGrab, focus, keybd))
            return;
    }

    /* just deliver it to the focus window */
    ptr = GetMaster(keybd, POINTER_OR_FLOAT);

    rc = EventToXI2(event, &xi2);
    if (rc == Success) {
        /* XXX: XACE */
        int filter = GetEventFilter(keybd, xi2);

        FixUpEventFromWindow(ptr->spriteInfo->sprite, xi2, focus, None, FALSE);
        deliveries = DeliverEventsToWindow(keybd, focus, xi2, 1,
                                           filter, NullGrab);
        if (deliveries > 0)
            goto unwind;
    }
    else if (rc != BadMatch)
        ErrorF
            ("[dix] %s: XI2 conversion failed in DFE (%d, %d). Skipping delivery.\n",
             keybd->name, event->any.type, rc);

    rc = EventToXI(event, &xE, &count);
    if (rc == Success &&
        XaceHook(XACE_SEND_ACCESS, NULL, keybd, focus, xE, count) == Success) {
        FixUpEventFromWindow(ptr->spriteInfo->sprite, xE, focus, None, FALSE);
        deliveries = DeliverEventsToWindow(keybd, focus, xE, count,
                                           GetEventFilter(keybd, xE), NullGrab);

        if (deliveries > 0)
            goto unwind;
    }
    else if (rc != BadMatch)
        ErrorF
            ("[dix] %s: XI conversion failed in DFE (%d, %d). Skipping delivery.\n",
             keybd->name, event->any.type, rc);

    if (sendCore) {
        rc = EventToCore(event, &core, &count);
        if (rc == Success) {
            if (XaceHook(XACE_SEND_ACCESS, NULL, keybd, focus, core, count) ==
                Success) {
                FixUpEventFromWindow(keybd->spriteInfo->sprite, core, focus,
                                     None, FALSE);
                deliveries =
                    DeliverEventsToWindow(keybd, focus, core, count,
                                          GetEventFilter(keybd, core),
                                          NullGrab);
            }
        }
        else if (rc != BadMatch)
            ErrorF
                ("[dix] %s: core conversion failed DFE (%d, %d). Skipping delivery.\n",
                 keybd->name, event->any.type, rc);
    }

 unwind:
    free(core);
    free(xE);
    free(xi2);
    return;
}
