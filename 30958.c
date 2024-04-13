GetEventMask(DeviceIntPtr dev, xEvent *event, InputClients * other)
{
    int evtype;

    /* XI2 filters are only ever 8 bit, so let's return a 8 bit mask */
    if ((evtype = xi2_get_type(event))) {
        return GetXI2MaskByte(other->xi2mask, dev, evtype);
    }
    else if (core_get_type(event) != 0)
        return other->mask[XIAllDevices];
    else
        return other->mask[dev->id];
}
