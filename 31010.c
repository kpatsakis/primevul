UpdateTouchesForGrab(DeviceIntPtr mouse)
{
    int i;

    if (!mouse->touch || mouse->deviceGrab.fromPassiveGrab)
        return;

    for (i = 0; i < mouse->touch->num_touches; i++) {
        TouchPointInfoPtr ti = mouse->touch->touches + i;
        TouchListener *listener = &ti->listeners[0];
        GrabPtr grab = mouse->deviceGrab.grab;

        if (ti->active &&
            CLIENT_BITS(listener->listener) == grab->resource) {
            listener->listener = grab->resource;
            listener->level = grab->grabtype;
            listener->state = LISTENER_IS_OWNER;
            listener->window = grab->window;

            if (grab->grabtype == CORE || grab->grabtype == XI ||
                !xi2mask_isset(grab->xi2mask, mouse, XI_TouchBegin))
                listener->type = LISTENER_POINTER_GRAB;
            else
                listener->type = LISTENER_GRAB;
            if (listener->grab)
                FreeGrab(listener->grab);
            listener->grab = AllocGrab(grab);
        }
    }
}
