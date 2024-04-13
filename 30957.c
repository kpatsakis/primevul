GetEventFilter(DeviceIntPtr dev, xEvent *event)
{
    int evtype = 0;

    if (event->u.u.type != GenericEvent)
        return event_get_filter_from_type(dev, event->u.u.type);
    else if ((evtype = xi2_get_type(event)))
        return event_get_filter_from_xi2type(evtype);
    ErrorF("[dix] Unknown event type %d. No filter\n", event->u.u.type);
    return 0;
}
