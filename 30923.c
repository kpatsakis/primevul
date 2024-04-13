ComputeFreezes(void)
{
    DeviceIntPtr replayDev = syncEvents.replayDev;
    WindowPtr w;
    GrabPtr grab;
    DeviceIntPtr dev;

    for (dev = inputInfo.devices; dev; dev = dev->next)
        FreezeThaw(dev, dev->deviceGrab.sync.other ||
                   (dev->deviceGrab.sync.state >= FROZEN));
    if (syncEvents.playingEvents ||
        (!replayDev && xorg_list_is_empty(&syncEvents.pending)))
        return;
    syncEvents.playingEvents = TRUE;
    if (replayDev) {
        DeviceEvent *event = replayDev->deviceGrab.sync.event;

        syncEvents.replayDev = (DeviceIntPtr) NULL;

        w = XYToWindow(replayDev->spriteInfo->sprite,
                       event->root_x, event->root_y);
        if (!CheckDeviceGrabs(replayDev, event, syncEvents.replayWin)) {
            if (IsTouchEvent((InternalEvent *) event)) {
                TouchPointInfoPtr ti =
                    TouchFindByClientID(replayDev, event->touchid);
                BUG_WARN(!ti);

                TouchListenerAcceptReject(replayDev, ti, 0, XIRejectTouch);
            }
            else if (replayDev->focus &&
                     !IsPointerEvent((InternalEvent *) event))
                DeliverFocusedEvent(replayDev, (InternalEvent *) event, w);
            else
                DeliverDeviceEvents(w, (InternalEvent *) event, NullGrab,
                                    NullWindow, replayDev);
        }
    }
    for (dev = inputInfo.devices; dev; dev = dev->next) {
        if (!dev->deviceGrab.sync.frozen) {
            PlayReleasedEvents();
            break;
        }
    }
    syncEvents.playingEvents = FALSE;
    for (dev = inputInfo.devices; dev; dev = dev->next) {
        if (DevHasCursor(dev)) {
            /* the following may have been skipped during replay,
               so do it now */
            if ((grab = dev->deviceGrab.grab) && grab->confineTo) {
                if (grab->confineTo->drawable.pScreen !=
                    dev->spriteInfo->sprite->hotPhys.pScreen)
                    dev->spriteInfo->sprite->hotPhys.x =
                        dev->spriteInfo->sprite->hotPhys.y = 0;
                ConfineCursorToWindow(dev, grab->confineTo, TRUE, TRUE);
            }
            else
                ConfineCursorToWindow(dev,
                                      dev->spriteInfo->sprite->hotPhys.pScreen->
                                      root, TRUE, FALSE);
            PostNewCursor(dev);
        }
    }
}
