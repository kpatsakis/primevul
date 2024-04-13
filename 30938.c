DeliverOneEvent(InternalEvent *event, DeviceIntPtr dev, enum InputLevel level,
                WindowPtr win, Window child, GrabPtr grab)
{
    xEvent *xE = NULL;
    int count = 0;
    int deliveries = 0;
    int rc;

    switch (level) {
    case XI2:
        rc = EventToXI2(event, &xE);
        count = 1;
        break;
    case XI:
        rc = EventToXI(event, &xE, &count);
        break;
    case CORE:
        rc = EventToCore(event, &xE, &count);
        break;
    default:
        rc = BadImplementation;
        break;
    }

    if (rc == Success) {
        deliveries = DeliverEvent(dev, xE, count, win, child, grab);
        free(xE);
    }
    else
        BUG_WARN_MSG(rc != BadMatch,
                     "%s: conversion to level %d failed with rc %d\n",
                     dev->name, level, rc);
    return deliveries;
}
