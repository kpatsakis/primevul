NoticeEventTime(InternalEvent *ev, DeviceIntPtr dev)
{
    if (!syncEvents.playingEvents)
        NoticeTimeMillis(dev, &ev->any.time);
}
