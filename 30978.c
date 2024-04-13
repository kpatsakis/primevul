NoticeTimeMillis(const DeviceIntPtr dev, CARD32 *ms)
{
    TimeStamp time;
    if (*ms < currentTime.milliseconds)
        MonthChangedOrBadTime(ms);
    time.months = currentTime.months;
    time.milliseconds = *ms;
    NoticeTime(dev, time);
}
