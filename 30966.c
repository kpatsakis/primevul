IsInterferingGrab(ClientPtr client, DeviceIntPtr dev, xEvent *event)
{
    DeviceIntPtr it = inputInfo.devices;

    switch (event->u.u.type) {
    case KeyPress:
    case KeyRelease:
    case ButtonPress:
    case ButtonRelease:
    case MotionNotify:
    case EnterNotify:
    case LeaveNotify:
        break;
    default:
        return FALSE;
    }

    if (dev->deviceGrab.grab && SameClient(dev->deviceGrab.grab, client))
        return FALSE;

    while (it) {
        if (it != dev) {
            if (it->deviceGrab.grab && SameClient(it->deviceGrab.grab, client)
                && !it->deviceGrab.fromPassiveGrab) {
                if ((IsPointerDevice(it) && IsPointerDevice(dev)) ||
                    (IsKeyboardDevice(it) && IsKeyboardDevice(dev)))
                    return TRUE;
            }
        }
        it = it->next;
    }

    return FALSE;
}
