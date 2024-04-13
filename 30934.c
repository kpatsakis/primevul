DeliverEvents(WindowPtr pWin, xEvent *xE, int count, WindowPtr otherParent)
{
    DeviceIntRec dummy;
    int deliveries;

#ifdef PANORAMIX
    if (!noPanoramiXExtension && pWin->drawable.pScreen->myNum)
        return count;
#endif

    if (!count)
        return 0;

    dummy.id = XIAllDevices;

    switch (xE->u.u.type) {
    case DestroyNotify:
    case UnmapNotify:
    case MapNotify:
    case MapRequest:
    case ReparentNotify:
    case ConfigureNotify:
    case ConfigureRequest:
    case GravityNotify:
    case CirculateNotify:
    case CirculateRequest:
        xE->u.destroyNotify.event = pWin->drawable.id;
        break;
    }

    switch (xE->u.u.type) {
    case DestroyNotify:
    case UnmapNotify:
    case MapNotify:
    case ReparentNotify:
    case ConfigureNotify:
    case GravityNotify:
    case CirculateNotify:
        break;
    default:
    {
        Mask filter;

        filter = GetEventFilter(&dummy, xE);
        return DeliverEventsToWindow(&dummy, pWin, xE, count, filter, NullGrab);
    }
    }

    deliveries = DeliverEventsToWindow(&dummy, pWin, xE, count,
                                       StructureNotifyMask, NullGrab);
    if (pWin->parent) {
        xE->u.destroyNotify.event = pWin->parent->drawable.id;
        deliveries += DeliverEventsToWindow(&dummy, pWin->parent, xE, count,
                                            SubstructureNotifyMask, NullGrab);
        if (xE->u.u.type == ReparentNotify) {
            xE->u.destroyNotify.event = otherParent->drawable.id;
            deliveries += DeliverEventsToWindow(&dummy,
                                                otherParent, xE, count,
                                                SubstructureNotifyMask,
                                                NullGrab);
        }
    }
    return deliveries;
}
