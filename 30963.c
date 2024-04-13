InitEvents(void)
{
    int i;
    QdEventPtr qe, tmp;

    inputInfo.numDevices = 0;
    inputInfo.devices = (DeviceIntPtr) NULL;
    inputInfo.off_devices = (DeviceIntPtr) NULL;
    inputInfo.keyboard = (DeviceIntPtr) NULL;
    inputInfo.pointer = (DeviceIntPtr) NULL;

    for (i = 0; i < MAXDEVICES; i++) {
        DeviceIntRec dummy;
        memcpy(&event_filters[i], default_filter, sizeof(default_filter));

        dummy.id = i;
        NoticeTime(&dummy, currentTime);
        LastEventTimeToggleResetFlag(i, FALSE);
    }

    syncEvents.replayDev = (DeviceIntPtr) NULL;
    syncEvents.replayWin = NullWindow;
    if (syncEvents.pending.next)
        xorg_list_for_each_entry_safe(qe, tmp, &syncEvents.pending, next)
            free(qe);
    xorg_list_init(&syncEvents.pending);
    syncEvents.playingEvents = FALSE;
    syncEvents.time.months = 0;
    syncEvents.time.milliseconds = 0;   /* hardly matters */
    currentTime.months = 0;
    currentTime.milliseconds = GetTimeInMillis();
    for (i = 0; i < DNPMCOUNT; i++) {
        DontPropagateMasks[i] = 0;
        DontPropagateRefCnts[i] = 0;
    }

    InputEventList = InitEventList(GetMaximumEventsNum());
    if (!InputEventList)
        FatalError("[dix] Failed to allocate input event list.\n");
}
