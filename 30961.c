GetXI2MaskByte(XI2Mask *mask, DeviceIntPtr dev, int event_type)
{
    /* we just return the matching filter because that's the only use
     * for this mask anyway.
     */
    if (xi2mask_isset(mask, dev, event_type))
        return event_get_filter_from_xi2type(event_type);
    else
        return 0;
}
