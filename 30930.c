DeliverDeviceEvents(WindowPtr pWin, InternalEvent *event, GrabPtr grab,
                    WindowPtr stopAt, DeviceIntPtr dev)
{
    Window child = None;
    int deliveries = 0;
    int mask;

    verify_internal_event(event);

    while (pWin) {
        if ((mask = EventIsDeliverable(dev, event->any.type, pWin))) {
            /* XI2 events first */
            if (mask & EVENT_XI2_MASK) {
                deliveries =
                    DeliverOneEvent(event, dev, XI2, pWin, child, grab);
                if (deliveries > 0)
                    break;
            }

            /* XI events */
            if (mask & EVENT_XI1_MASK) {
                deliveries = DeliverOneEvent(event, dev, XI, pWin, child, grab);
                if (deliveries > 0)
                    break;
            }

            /* Core event */
            if ((mask & EVENT_CORE_MASK) && IsMaster(dev) && dev->coreEvents) {
                deliveries =
                    DeliverOneEvent(event, dev, CORE, pWin, child, grab);
                if (deliveries > 0)
                    break;
            }

        }

        if ((deliveries < 0) || (pWin == stopAt) ||
            (mask & EVENT_DONT_PROPAGATE_MASK)) {
            deliveries = 0;
            break;
        }

        child = pWin->drawable.id;
        pWin = pWin->parent;
    }

    return deliveries;
}
