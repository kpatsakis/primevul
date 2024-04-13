ActivateEnterGrab(DeviceIntPtr dev, WindowPtr old, WindowPtr win)
{
    BOOL rc = FALSE;
    DeviceEvent event;

    if (dev->deviceGrab.grab) {
        if (!dev->deviceGrab.fromPassiveGrab ||
            dev->deviceGrab.grab->type != XI_Enter ||
            dev->deviceGrab.grab->window == win ||
            IsParent(dev->deviceGrab.grab->window, win))
            return FALSE;
        DoEnterLeaveEvents(dev, dev->id, old, win, XINotifyPassiveUngrab);
        (*dev->deviceGrab.DeactivateGrab) (dev);
    }

    event = (DeviceEvent) {
        .header = ET_Internal,
        .type = ET_Enter,
        .length = sizeof(DeviceEvent),
        .time = GetTimeInMillis(),
        .deviceid = dev->id,
        .sourceid = dev->id,
        .detail.button = 0
    };
    rc = (CheckPassiveGrabsOnWindow(win, dev, (InternalEvent *) &event, FALSE,
                                    TRUE) != NULL);
    if (rc)
        DoEnterLeaveEvents(dev, dev->id, old, win, XINotifyPassiveGrab);
    return rc;
}
