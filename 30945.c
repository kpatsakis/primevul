EnqueueEvent(InternalEvent *ev, DeviceIntPtr device)
{
    QdEventPtr tail = NULL;
    QdEventPtr qe;
    SpritePtr pSprite = device->spriteInfo->sprite;
    int eventlen;
    DeviceEvent *event = &ev->device_event;

    if (!xorg_list_is_empty(&syncEvents.pending))
        tail = xorg_list_last_entry(&syncEvents.pending, QdEventRec, next);

    NoticeTimeMillis(device, &ev->any.time);

    /* Fix for key repeating bug. */
    if (device->key != NULL && device->key->xkbInfo != NULL &&
        event->type == ET_KeyRelease)
        AccessXCancelRepeatKey(device->key->xkbInfo, event->detail.key);

    if (DeviceEventCallback) {
        DeviceEventInfoRec eventinfo;

        /*  The RECORD spec says that the root window field of motion events
         *  must be valid.  At this point, it hasn't been filled in yet, so
         *  we do it here.  The long expression below is necessary to get
         *  the current root window; the apparently reasonable alternative
         *  GetCurrentRootWindow()->drawable.id doesn't give you the right
         *  answer on the first motion event after a screen change because
         *  the data that GetCurrentRootWindow relies on hasn't been
         *  updated yet.
         */
        if (ev->any.type == ET_Motion)
            ev->device_event.root = pSprite->hotPhys.pScreen->root->drawable.id;

        eventinfo.event = ev;
        eventinfo.device = device;
        CallCallbacks(&DeviceEventCallback, (void *) &eventinfo);
    }

    if (event->type == ET_Motion) {
#ifdef PANORAMIX
        if (!noPanoramiXExtension) {
            event->root_x += pSprite->screen->x - screenInfo.screens[0]->x;
            event->root_y += pSprite->screen->y - screenInfo.screens[0]->y;
        }
#endif
        pSprite->hotPhys.x = event->root_x;
        pSprite->hotPhys.y = event->root_y;
        /* do motion compression, but not if from different devices */
        if (tail &&
            (tail->event->any.type == ET_Motion) &&
            (tail->device == device) &&
            (tail->pScreen == pSprite->hotPhys.pScreen)) {
            DeviceEvent *tailev = &tail->event->device_event;

            tailev->root_x = pSprite->hotPhys.x;
            tailev->root_y = pSprite->hotPhys.y;
            tailev->time = event->time;
            tail->months = currentTime.months;
            return;
        }
    }

    eventlen = event->length;

    qe = malloc(sizeof(QdEventRec) + eventlen);
    if (!qe)
        return;
    xorg_list_init(&qe->next);
    qe->device = device;
    qe->pScreen = pSprite->hotPhys.pScreen;
    qe->months = currentTime.months;
    qe->event = (InternalEvent *) (qe + 1);
    memcpy(qe->event, event, eventlen);
    xorg_list_append(&qe->next, &syncEvents.pending);
}
