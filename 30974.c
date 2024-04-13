MaybeDeliverEventsToClient(WindowPtr pWin, xEvent *pEvents,
                           int count, Mask filter, ClientPtr dontClient)
{
    OtherClients *other;

    if (pWin->eventMask & filter) {
        if (wClient(pWin) == dontClient)
            return 0;
#ifdef PANORAMIX
        if (!noPanoramiXExtension && pWin->drawable.pScreen->myNum)
            return XineramaTryClientEventsResult(wClient(pWin), NullGrab,
                                                 pWin->eventMask, filter);
#endif
        if (XaceHook(XACE_RECEIVE_ACCESS, wClient(pWin), pWin, pEvents, count))
            return 1;           /* don't send, but pretend we did */
        return TryClientEvents(wClient(pWin), NULL, pEvents, count,
                               pWin->eventMask, filter, NullGrab);
    }
    for (other = wOtherClients(pWin); other; other = other->next) {
        if (other->mask & filter) {
            if (SameClient(other, dontClient))
                return 0;
#ifdef PANORAMIX
            if (!noPanoramiXExtension && pWin->drawable.pScreen->myNum)
                return XineramaTryClientEventsResult(rClient(other), NullGrab,
                                                     other->mask, filter);
#endif
            if (XaceHook(XACE_RECEIVE_ACCESS, rClient(other), pWin, pEvents,
                         count))
                return 1;       /* don't send, but pretend we did */
            return TryClientEvents(rClient(other), NULL, pEvents, count,
                                   other->mask, filter, NullGrab);
        }
    }
    return 2;
}
