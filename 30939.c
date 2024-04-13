DeliverOneGrabbedEvent(InternalEvent *event, DeviceIntPtr dev,
                       enum InputLevel level)
{
    SpritePtr pSprite = dev->spriteInfo->sprite;
    int rc;
    xEvent *xE = NULL;
    int count = 0;
    int deliveries = 0;
    Mask mask;
    GrabInfoPtr grabinfo = &dev->deviceGrab;
    GrabPtr grab = grabinfo->grab;
    Mask filter;

    if (grab->grabtype != level)
        return 0;

    switch (level) {
    case XI2:
        rc = EventToXI2(event, &xE);
        count = 1;
        if (rc == Success) {
            int evtype = xi2_get_type(xE);

            mask = GetXI2MaskByte(grab->xi2mask, dev, evtype);
            filter = GetEventFilter(dev, xE);
        }
        break;
    case XI:
        if (grabinfo->fromPassiveGrab && grabinfo->implicitGrab)
            mask = grab->deviceMask;
        else
            mask = grab->eventMask;
        rc = EventToXI(event, &xE, &count);
        if (rc == Success)
            filter = GetEventFilter(dev, xE);
        break;
    case CORE:
        rc = EventToCore(event, &xE, &count);
        mask = grab->eventMask;
        if (rc == Success)
            filter = GetEventFilter(dev, xE);
        break;
    default:
        BUG_WARN_MSG(1, "Invalid input level %d\n", level);
        return 0;
    }

    if (rc == Success) {
        FixUpEventFromWindow(pSprite, xE, grab->window, None, TRUE);
        if (XaceHook(XACE_SEND_ACCESS, 0, dev,
                     grab->window, xE, count) ||
            XaceHook(XACE_RECEIVE_ACCESS, rClient(grab),
                     grab->window, xE, count))
            deliveries = 1;     /* don't send, but pretend we did */
        else if (level != CORE || !IsInterferingGrab(rClient(grab), dev, xE)) {
            deliveries = TryClientEvents(rClient(grab), dev,
                                         xE, count, mask, filter, grab);
        }
    }
    else
        BUG_WARN_MSG(rc != BadMatch,
                     "%s: conversion to mode %d failed on %d with %d\n",
                     dev->name, level, event->any.type, rc);

    free(xE);
    return deliveries;
}
