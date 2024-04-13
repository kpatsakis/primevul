FreezeThaw(DeviceIntPtr dev, Bool frozen)
{
    dev->deviceGrab.sync.frozen = frozen;
    if (frozen)
        dev->public.processInputProc = dev->public.enqueueInputProc;
    else
        dev->public.processInputProc = dev->public.realInputProc;
}
