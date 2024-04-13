DeactivatePointerGrab(DeviceIntPtr mouse)
{
    GrabPtr grab = mouse->deviceGrab.grab;
    DeviceIntPtr dev;
    Bool wasPassive = mouse->deviceGrab.fromPassiveGrab;
    Bool wasImplicit = (mouse->deviceGrab.fromPassiveGrab &&
                        mouse->deviceGrab.implicitGrab);
    XID grab_resource = grab->resource;
    int i;

    /* If an explicit grab was deactivated, we must remove it from the head of
     * all the touches' listener lists. */
    for (i = 0; !wasPassive && mouse->touch && i < mouse->touch->num_touches; i++) {
        TouchPointInfoPtr ti = mouse->touch->touches + i;
        if (ti->active && TouchResourceIsOwner(ti, grab_resource)) {
            int mode = XIRejectTouch;
            /* Rejecting will generate a TouchEnd, but we must not
               emulate a ButtonRelease here. So pretend the listener
               already has the end event */
            if (grab->grabtype == CORE || grab->grabtype == XI ||
                    !xi2mask_isset(mouse->deviceGrab.grab->xi2mask, mouse, XI_TouchBegin)) {
                mode = XIAcceptTouch;
                /* NOTE: we set the state here, but
                 * ProcessTouchOwnershipEvent() will still call
                 * TouchEmitTouchEnd for this listener. The other half of
                 * this hack is in DeliverTouchEndEvent */
                ti->listeners[0].state = LISTENER_HAS_END;
            }
            TouchListenerAcceptReject(mouse, ti, 0, mode);
        }
    }

    TouchRemovePointerGrab(mouse);

    mouse->valuator->motionHintWindow = NullWindow;
    mouse->deviceGrab.grab = NullGrab;
    mouse->deviceGrab.sync.state = NOT_GRABBED;
    mouse->deviceGrab.fromPassiveGrab = FALSE;

    for (dev = inputInfo.devices; dev; dev = dev->next) {
        if (dev->deviceGrab.sync.other == grab)
            dev->deviceGrab.sync.other = NullGrab;
    }
    DoEnterLeaveEvents(mouse, mouse->id, grab->window,
                       mouse->spriteInfo->sprite->win, NotifyUngrab);
    if (grab->confineTo)
        ConfineCursorToWindow(mouse, GetCurrentRootWindow(mouse), FALSE, FALSE);
    PostNewCursor(mouse);

    if (!wasImplicit && grab->grabtype == XI2)
        ReattachToOldMaster(mouse);

    ComputeFreezes();

    FreeGrab(grab);
}
