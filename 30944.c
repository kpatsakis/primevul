DeviceEnterLeaveEvent(DeviceIntPtr mouse,
                      int sourceid,
                      int type,
                      int mode, int detail, WindowPtr pWin, Window child)
{
    GrabPtr grab = mouse->deviceGrab.grab;
    xXIEnterEvent *event;
    WindowPtr focus;
    int filter;
    int btlen, len, i;
    DeviceIntPtr kbd;

    if ((mode == XINotifyPassiveGrab && type == XI_Leave) ||
        (mode == XINotifyPassiveUngrab && type == XI_Enter))
        return;

    btlen = (mouse->button) ? bits_to_bytes(mouse->button->numButtons) : 0;
    btlen = bytes_to_int32(btlen);
    len = sizeof(xXIEnterEvent) + btlen * 4;

    event = calloc(1, len);
    event->type = GenericEvent;
    event->extension = IReqCode;
    event->evtype = type;
    event->length = (len - sizeof(xEvent)) / 4;
    event->buttons_len = btlen;
    event->detail = detail;
    event->time = currentTime.milliseconds;
    event->deviceid = mouse->id;
    event->sourceid = sourceid;
    event->mode = mode;
    event->root_x = double_to_fp1616(mouse->spriteInfo->sprite->hot.x);
    event->root_y = double_to_fp1616(mouse->spriteInfo->sprite->hot.y);

    for (i = 0; mouse && mouse->button && i < mouse->button->numButtons; i++)
        if (BitIsOn(mouse->button->down, i))
            SetBit(&event[1], i);

    kbd = GetMaster(mouse, MASTER_KEYBOARD);
    if (kbd && kbd->key) {
        event->mods.base_mods = kbd->key->xkbInfo->state.base_mods;
        event->mods.latched_mods = kbd->key->xkbInfo->state.latched_mods;
        event->mods.locked_mods = kbd->key->xkbInfo->state.locked_mods;

        event->group.base_group = kbd->key->xkbInfo->state.base_group;
        event->group.latched_group = kbd->key->xkbInfo->state.latched_group;
        event->group.locked_group = kbd->key->xkbInfo->state.locked_group;
    }

    focus = (kbd) ? kbd->focus->win : None;
    if ((focus != NoneWin) &&
        ((pWin == focus) || (focus == PointerRootWin) || IsParent(focus, pWin)))
        event->focus = TRUE;

    FixUpEventFromWindow(mouse->spriteInfo->sprite, (xEvent *) event, pWin,
                         None, FALSE);

    filter = GetEventFilter(mouse, (xEvent *) event);

    if (grab && grab->grabtype == XI2) {
        Mask mask;

        mask = xi2mask_isset(grab->xi2mask, mouse, type);
        TryClientEvents(rClient(grab), mouse, (xEvent *) event, 1, mask, 1,
                        grab);
    }
    else {
        if (!WindowXI2MaskIsset(mouse, pWin, (xEvent *) event))
            goto out;
        DeliverEventsToWindow(mouse, pWin, (xEvent *) event, 1, filter,
                              NullGrab);
    }

 out:
    free(event);
}
