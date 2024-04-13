PlayReleasedEvents(void)
{
    QdEventPtr tmp;
    QdEventPtr qe;
    DeviceIntPtr dev;
    DeviceIntPtr pDev;

 restart:
    xorg_list_for_each_entry_safe(qe, tmp, &syncEvents.pending, next) {
        if (!qe->device->deviceGrab.sync.frozen) {
            xorg_list_del(&qe->next);
            pDev = qe->device;
            if (qe->event->any.type == ET_Motion)
                CheckVirtualMotion(pDev, qe, NullWindow);
            syncEvents.time.months = qe->months;
            syncEvents.time.milliseconds = qe->event->any.time;
#ifdef PANORAMIX
            /* Translate back to the sprite screen since processInputProc
               will translate from sprite screen to screen 0 upon reentry
               to the DIX layer */
            if (!noPanoramiXExtension) {
                DeviceEvent *ev = &qe->event->device_event;

                switch (ev->type) {
                case ET_Motion:
                case ET_ButtonPress:
                case ET_ButtonRelease:
                case ET_KeyPress:
                case ET_KeyRelease:
                case ET_ProximityIn:
                case ET_ProximityOut:
                case ET_TouchBegin:
                case ET_TouchUpdate:
                case ET_TouchEnd:
                    ev->root_x += screenInfo.screens[0]->x -
                        pDev->spriteInfo->sprite->screen->x;
                    ev->root_y += screenInfo.screens[0]->y -
                        pDev->spriteInfo->sprite->screen->y;
                    break;
                default:
                    break;
                }

            }
#endif
            (*qe->device->public.processInputProc) (qe->event, qe->device);
            free(qe);
            for (dev = inputInfo.devices; dev && dev->deviceGrab.sync.frozen;
                 dev = dev->next);
            if (!dev)
                break;

            /* Playing the event may have unfrozen another device. */
            /* So to play it safe, restart at the head of the queue */
            goto restart;
        }
    }
}
