CheckGrabForSyncs(DeviceIntPtr thisDev, Bool thisMode, Bool otherMode)
{
    GrabPtr grab = thisDev->deviceGrab.grab;
    DeviceIntPtr dev;

    if (thisMode == GrabModeSync)
        thisDev->deviceGrab.sync.state = FROZEN_NO_EVENT;
    else {                      /* free both if same client owns both */
        thisDev->deviceGrab.sync.state = THAWED;
        if (thisDev->deviceGrab.sync.other &&
            (CLIENT_BITS(thisDev->deviceGrab.sync.other->resource) ==
             CLIENT_BITS(grab->resource)))
            thisDev->deviceGrab.sync.other = NullGrab;
    }

    if (IsMaster(thisDev)) {
        dev = GetPairedDevice(thisDev);
        if (otherMode == GrabModeSync)
            dev->deviceGrab.sync.other = grab;
        else {                  /* free both if same client owns both */
            if (dev->deviceGrab.sync.other &&
                (CLIENT_BITS(dev->deviceGrab.sync.other->resource) ==
                 CLIENT_BITS(grab->resource)))
                dev->deviceGrab.sync.other = NullGrab;
        }
    }
    ComputeFreezes();
}
