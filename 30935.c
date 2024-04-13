DeliverEventsToWindow(DeviceIntPtr pDev, WindowPtr pWin, xEvent
                      *pEvents, int count, Mask filter, GrabPtr grab)
{
    int deliveries = 0, nondeliveries = 0;
    ClientPtr client = NullClient;
    Mask deliveryMask = 0;      /* If a grab occurs due to a button press, then
                                   this mask is the mask of the grab. */
    int type = pEvents->u.u.type;

    /* Deliver to window owner */
    if ((filter == CantBeFiltered) || core_get_type(pEvents) != 0) {
        enum EventDeliveryState rc;

        rc = DeliverToWindowOwner(pDev, pWin, pEvents, count, filter, grab);

        switch (rc) {
        case EVENT_SKIP:
            return 0;
        case EVENT_REJECTED:
            nondeliveries--;
            break;
        case EVENT_DELIVERED:
            /* We delivered to the owner, with our event mask */
            deliveries++;
            client = wClient(pWin);
            deliveryMask = pWin->eventMask;
            break;
        case EVENT_NOT_DELIVERED:
            break;
        }
    }

    /* CantBeFiltered means only window owner gets the event */
    if (filter != CantBeFiltered) {
        enum EventDeliveryState rc;

        rc = DeliverEventToWindowMask(pDev, pWin, pEvents, count, filter,
                                      grab, &client, &deliveryMask);

        switch (rc) {
        case EVENT_SKIP:
            return 0;
        case EVENT_REJECTED:
            nondeliveries--;
            break;
        case EVENT_DELIVERED:
            deliveries++;
            break;
        case EVENT_NOT_DELIVERED:
            break;
        }
    }

    if (deliveries) {
        /*
         * Note that since core events are delivered first, an implicit grab may
         * be activated on a core grab, stopping the XI events.
         */
        if (!grab &&
            ActivateImplicitGrab(pDev, client, pWin, pEvents, deliveryMask))
            /* grab activated */ ;
        else if (type == MotionNotify)
            pDev->valuator->motionHintWindow = pWin;
        else if (type == DeviceMotionNotify || type == DeviceButtonPress)
            CheckDeviceGrabAndHintWindow(pWin, type,
                                         (deviceKeyButtonPointer *) pEvents,
                                         grab, client, deliveryMask);
        return deliveries;
    }
    return nondeliveries;
}
