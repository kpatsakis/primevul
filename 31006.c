SetInputFocus(ClientPtr client,
              DeviceIntPtr dev,
              Window focusID, CARD8 revertTo, Time ctime, Bool followOK)
{
    FocusClassPtr focus;
    WindowPtr focusWin;
    int mode, rc;
    TimeStamp time;
    DeviceIntPtr keybd;         /* used for FollowKeyboard or FollowKeyboardWin */

    UpdateCurrentTime();
    if ((revertTo != RevertToParent) &&
        (revertTo != RevertToPointerRoot) &&
        (revertTo != RevertToNone) &&
        ((revertTo != RevertToFollowKeyboard) || !followOK)) {
        client->errorValue = revertTo;
        return BadValue;
    }
    time = ClientTimeToServerTime(ctime);

    keybd = GetMaster(dev, KEYBOARD_OR_FLOAT);

    if ((focusID == None) || (focusID == PointerRoot))
        focusWin = (WindowPtr) (long) focusID;
    else if ((focusID == FollowKeyboard) && followOK) {
        focusWin = keybd->focus->win;
    }
    else {
        rc = dixLookupWindow(&focusWin, focusID, client, DixSetAttrAccess);
        if (rc != Success)
            return rc;
        /* It is a match error to try to set the input focus to an
           unviewable window. */
        if (!focusWin->realized)
            return BadMatch;
    }
    rc = XaceHook(XACE_DEVICE_ACCESS, client, dev, DixSetFocusAccess);
    if (rc != Success)
        return Success;

    focus = dev->focus;
    if ((CompareTimeStamps(time, currentTime) == LATER) ||
        (CompareTimeStamps(time, focus->time) == EARLIER))
        return Success;
    mode = (dev->deviceGrab.grab) ? NotifyWhileGrabbed : NotifyNormal;
    if (focus->win == FollowKeyboardWin) {
        if (!ActivateFocusInGrab(dev, keybd->focus->win, focusWin))
            DoFocusEvents(dev, keybd->focus->win, focusWin, mode);
    }
    else {
        if (!ActivateFocusInGrab(dev, focus->win, focusWin))
            DoFocusEvents(dev, focus->win, focusWin, mode);
    }
    focus->time = time;
    focus->revert = revertTo;
    if (focusID == FollowKeyboard)
        focus->win = FollowKeyboardWin;
    else
        focus->win = focusWin;
    if ((focusWin == NoneWin) || (focusWin == PointerRootWin))
        focus->traceGood = 0;
    else {
        int depth = 0;
        WindowPtr pWin;

        for (pWin = focusWin; pWin; pWin = pWin->parent)
            depth++;
        if (depth > focus->traceSize) {
            focus->traceSize = depth + 1;
            focus->trace = reallocarray(focus->trace, focus->traceSize,
                                        sizeof(WindowPtr));
        }
        focus->traceGood = depth;
        for (pWin = focusWin, depth--; pWin; pWin = pWin->parent, depth--)
            focus->trace[depth] = pWin;
    }
    return Success;
}
