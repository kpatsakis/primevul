RecalculateDeliverableEvents(WindowPtr pWin)
{
    OtherClients *others;
    WindowPtr pChild;

    pChild = pWin;
    while (1) {
        if (pChild->optional) {
            pChild->optional->otherEventMasks = 0;
            for (others = wOtherClients(pChild); others; others = others->next) {
                pChild->optional->otherEventMasks |= others->mask;
            }
        }
        pChild->deliverableEvents = pChild->eventMask |
            wOtherEventMasks(pChild);
        if (pChild->parent)
            pChild->deliverableEvents |=
                (pChild->parent->deliverableEvents &
                 ~wDontPropagateMask(pChild) & PropagateMask);
        if (pChild->firstChild) {
            pChild = pChild->firstChild;
            continue;
        }
        while (!pChild->nextSib && (pChild != pWin))
            pChild = pChild->parent;
        if (pChild == pWin)
            break;
        pChild = pChild->nextSib;
    }
}
