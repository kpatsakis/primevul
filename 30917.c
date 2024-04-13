CheckDeviceGrabs(DeviceIntPtr device, DeviceEvent *event, WindowPtr ancestor)
{
    int i;
    WindowPtr pWin = NULL;
    FocusClassPtr focus =
        IsPointerEvent((InternalEvent *) event) ? NULL : device->focus;
    BOOL sendCore = (IsMaster(device) && device->coreEvents);
    Bool ret = FALSE;

    if (event->type != ET_ButtonPress && event->type != ET_KeyPress)
        return FALSE;

    if (event->type == ET_ButtonPress && (device->button->buttonsDown != 1))
        return FALSE;

    if (device->deviceGrab.grab)
        return FALSE;

    i = 0;
    if (ancestor) {
        while (i < device->spriteInfo->sprite->spriteTraceGood)
            if (device->spriteInfo->sprite->spriteTrace[i++] == ancestor)
                break;
        if (i == device->spriteInfo->sprite->spriteTraceGood)
            goto out;
    }

    if (focus) {
        for (; i < focus->traceGood; i++) {
            pWin = focus->trace[i];
            if (CheckPassiveGrabsOnWindow(pWin, device, (InternalEvent *) event,
                                          sendCore, TRUE)) {
                ret = TRUE;
                goto out;
            }
        }

        if ((focus->win == NoneWin) ||
            (i >= device->spriteInfo->sprite->spriteTraceGood) ||
            (pWin && pWin != device->spriteInfo->sprite->spriteTrace[i - 1]))
            goto out;
    }

    for (; i < device->spriteInfo->sprite->spriteTraceGood; i++) {
        pWin = device->spriteInfo->sprite->spriteTrace[i];
        if (CheckPassiveGrabsOnWindow(pWin, device, (InternalEvent *) event,
                                      sendCore, TRUE)) {
            ret = TRUE;
            goto out;
        }
    }

 out:
    if (ret == TRUE && event->type == ET_KeyPress)
        device->deviceGrab.activatingKey = event->detail.key;
    return ret;
}
