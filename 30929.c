DeleteWindowFromAnyEvents(WindowPtr pWin, Bool freeResources)
{
    WindowPtr parent;
    DeviceIntPtr mouse = inputInfo.pointer;
    DeviceIntPtr keybd = inputInfo.keyboard;
    FocusClassPtr focus;
    OtherClientsPtr oc;
    GrabPtr passive;
    GrabPtr grab;

    /* Deactivate any grabs performed on this window, before making any
       input focus changes. */
    grab = mouse->deviceGrab.grab;
    if (grab && ((grab->window == pWin) || (grab->confineTo == pWin)))
        (*mouse->deviceGrab.DeactivateGrab) (mouse);

    /* Deactivating a keyboard grab should cause focus events. */
    grab = keybd->deviceGrab.grab;
    if (grab && (grab->window == pWin))
        (*keybd->deviceGrab.DeactivateGrab) (keybd);

    /* And now the real devices */
    for (mouse = inputInfo.devices; mouse; mouse = mouse->next) {
        grab = mouse->deviceGrab.grab;
        if (grab && ((grab->window == pWin) || (grab->confineTo == pWin)))
            (*mouse->deviceGrab.DeactivateGrab) (mouse);
    }

    for (keybd = inputInfo.devices; keybd; keybd = keybd->next) {
        if (IsKeyboardDevice(keybd)) {
            focus = keybd->focus;

            /* If the focus window is a root window (ie. has no parent)
               then don't delete the focus from it. */

            if ((pWin == focus->win) && (pWin->parent != NullWindow)) {
                int focusEventMode = NotifyNormal;

                /* If a grab is in progress, then alter the mode of focus events. */

                if (keybd->deviceGrab.grab)
                    focusEventMode = NotifyWhileGrabbed;

                switch (focus->revert) {
                case RevertToNone:
                    DoFocusEvents(keybd, pWin, NoneWin, focusEventMode);
                    focus->win = NoneWin;
                    focus->traceGood = 0;
                    break;
                case RevertToParent:
                    parent = pWin;
                    do {
                        parent = parent->parent;
                        focus->traceGood--;
                    } while (!parent->realized
                    /* This would be a good protocol change -- windows being
                       reparented during SaveSet processing would cause the
                       focus to revert to the nearest enclosing window which
                       will survive the death of the exiting client, instead
                       of ending up reverting to a dying window and thence
                       to None */
#ifdef NOTDEF
                             || wClient(parent)->clientGone
#endif
                        );
                    if (!ActivateFocusInGrab(keybd, pWin, parent))
                        DoFocusEvents(keybd, pWin, parent, focusEventMode);
                    focus->win = parent;
                    focus->revert = RevertToNone;
                    break;
                case RevertToPointerRoot:
                    if (!ActivateFocusInGrab(keybd, pWin, PointerRootWin))
                        DoFocusEvents(keybd, pWin, PointerRootWin,
                                      focusEventMode);
                    focus->win = PointerRootWin;
                    focus->traceGood = 0;
                    break;
                }
            }
        }

        if (IsPointerDevice(keybd)) {
            if (keybd->valuator->motionHintWindow == pWin)
                keybd->valuator->motionHintWindow = NullWindow;
        }
    }

    if (freeResources) {
        if (pWin->dontPropagate)
            DontPropagateRefCnts[pWin->dontPropagate]--;
        while ((oc = wOtherClients(pWin)))
            FreeResource(oc->resource, RT_NONE);
        while ((passive = wPassiveGrabs(pWin)))
            FreeResource(passive->resource, RT_NONE);
    }

    DeleteWindowFromAnyExtEvents(pWin, freeResources);
}
