SetMaskForEvent(int deviceid, Mask mask, int event)
{
    if (deviceid < 0 || deviceid >= MAXDEVICES)
        FatalError("SetMaskForEvent: bogus device id");
    event_filters[deviceid][event] = mask;
}
