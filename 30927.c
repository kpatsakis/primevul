DeactivateKeyboardGrab(DeviceIntPtr keybd)
{
    GrabPtr grab = keybd->deviceGrab.grab;
    DeviceIntPtr dev;
    WindowPtr focusWin;
    Bool wasImplicit = (keybd->deviceGrab.fromPassiveGrab &&
                        keybd->deviceGrab.implicitGrab);

    if (keybd->valuator)
        keybd->valuator->motionHintWindow = NullWindow;
    keybd->deviceGrab.grab = NullGrab;
    keybd->deviceGrab.sync.state = NOT_GRABBED;
    keybd->deviceGrab.fromPassiveGrab = FALSE;

    for (dev = inputInfo.devices; dev; dev = dev->next) {
        if (dev->deviceGrab.sync.other == grab)
            dev->deviceGrab.sync.other = NullGrab;
    }

    if (keybd->focus)
        focusWin = keybd->focus->win;
    else if (keybd->spriteInfo->sprite)
        focusWin = keybd->spriteInfo->sprite->win;
    else
        focusWin = NullWindow;

    if (focusWin == FollowKeyboardWin)
        focusWin = inputInfo.keyboard->focus->win;

    DoFocusEvents(keybd, grab->window, focusWin, NotifyUngrab);

    if (!wasImplicit && grab->grabtype == XI2)
        ReattachToOldMaster(keybd);

    ComputeFreezes();

    FreeGrab(grab);
}
