EventIsDeliverable(DeviceIntPtr dev, int evtype, WindowPtr win)
{
    int rc = 0;
    int filter = 0;
    int type;
    OtherInputMasks *inputMasks = wOtherInputMasks(win);

    if ((type = GetXI2Type(evtype)) != 0) {
        if (inputMasks && xi2mask_isset(inputMasks->xi2mask, dev, type))
            rc |= EVENT_XI2_MASK;
    }

    if ((type = GetXIType(evtype)) != 0) {
        filter = event_get_filter_from_type(dev, type);

        /* Check for XI mask */
        if (inputMasks &&
            (inputMasks->deliverableEvents[dev->id] & filter) &&
            (inputMasks->inputEvents[dev->id] & filter))
            rc |= EVENT_XI1_MASK;

        /* Check for XI DontPropagate mask */
        if (inputMasks && (inputMasks->dontPropagateMask[dev->id] & filter))
            rc |= EVENT_DONT_PROPAGATE_MASK;

    }

    if ((type = GetCoreType(evtype)) != 0) {
        filter = event_get_filter_from_type(dev, type);

        /* Check for core mask */
        if ((win->deliverableEvents & filter) &&
            ((wOtherEventMasks(win) | win->eventMask) & filter))
            rc |= EVENT_CORE_MASK;

        /* Check for core DontPropagate mask */
        if (filter & wDontPropagateMask(win))
            rc |= EVENT_DONT_PROPAGATE_MASK;
    }

    return rc;
}
