EventSelectForWindow(WindowPtr pWin, ClientPtr client, Mask mask)
{
    Mask check;
    OtherClients *others;
    DeviceIntPtr dev;
    int rc;

    if (mask & ~AllEventMasks) {
        client->errorValue = mask;
        return BadValue;
    }
    check = (mask & ManagerMask);
    if (check) {
        rc = XaceHook(XACE_RESOURCE_ACCESS, client, pWin->drawable.id,
                      RT_WINDOW, pWin, RT_NONE, NULL, DixManageAccess);
        if (rc != Success)
            return rc;
    }
    check = (mask & AtMostOneClient);
    if (check & (pWin->eventMask | wOtherEventMasks(pWin))) {
        /* It is illegal for two different clients to select on any of the
           events for AtMostOneClient. However, it is OK, for some client to
           continue selecting on one of those events.  */
        if ((wClient(pWin) != client) && (check & pWin->eventMask))
            return BadAccess;
        for (others = wOtherClients(pWin); others; others = others->next) {
            if (!SameClient(others, client) && (check & others->mask))
                return BadAccess;
        }
    }
    if (wClient(pWin) == client) {
        check = pWin->eventMask;
        pWin->eventMask = mask;
    }
    else {
        for (others = wOtherClients(pWin); others; others = others->next) {
            if (SameClient(others, client)) {
                check = others->mask;
                if (mask == 0) {
                    FreeResource(others->resource, RT_NONE);
                    return Success;
                }
                else
                    others->mask = mask;
                goto maskSet;
            }
        }
        check = 0;
        if (!pWin->optional && !MakeWindowOptional(pWin))
            return BadAlloc;
        others = malloc(sizeof(OtherClients));
        if (!others)
            return BadAlloc;
        others->mask = mask;
        others->resource = FakeClientID(client->index);
        others->next = pWin->optional->otherClients;
        pWin->optional->otherClients = others;
        if (!AddResource(others->resource, RT_OTHERCLIENT, (void *) pWin))
            return BadAlloc;
    }
 maskSet:
    if ((mask & PointerMotionHintMask) && !(check & PointerMotionHintMask)) {
        for (dev = inputInfo.devices; dev; dev = dev->next) {
            if (dev->valuator && dev->valuator->motionHintWindow == pWin)
                dev->valuator->motionHintWindow = NullWindow;
        }
    }
    RecalculateDeliverableEvents(pWin);
    return Success;
}
