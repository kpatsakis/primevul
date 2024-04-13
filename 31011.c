WindowXI2MaskIsset(DeviceIntPtr dev, WindowPtr win, xEvent *ev)
{
    OtherInputMasks *inputMasks = wOtherInputMasks(win);
    int evtype;

    if (!inputMasks || xi2_get_type(ev) == 0)
        return 0;

    evtype = ((xGenericEvent *) ev)->evtype;

    return xi2mask_isset(inputMasks->xi2mask, dev, evtype);
}
