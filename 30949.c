EventSuppressForWindow(WindowPtr pWin, ClientPtr client,
                       Mask mask, Bool *checkOptional)
{
    int i, freed;

    if (mask & ~PropagateMask) {
        client->errorValue = mask;
        return BadValue;
    }
    if (pWin->dontPropagate)
        DontPropagateRefCnts[pWin->dontPropagate]--;
    if (!mask)
        i = 0;
    else {
        for (i = DNPMCOUNT, freed = 0; --i > 0;) {
            if (!DontPropagateRefCnts[i])
                freed = i;
            else if (mask == DontPropagateMasks[i])
                break;
        }
        if (!i && freed) {
            i = freed;
            DontPropagateMasks[i] = mask;
        }
    }
    if (i || !mask) {
        pWin->dontPropagate = i;
        if (i)
            DontPropagateRefCnts[i]++;
        if (pWin->optional) {
            pWin->optional->dontPropagateMask = mask;
            *checkOptional = TRUE;
        }
    }
    else {
        if (!pWin->optional && !MakeWindowOptional(pWin)) {
            if (pWin->dontPropagate)
                DontPropagateRefCnts[pWin->dontPropagate]++;
            return BadAlloc;
        }
        pWin->dontPropagate = 0;
        pWin->optional->dontPropagateMask = mask;
    }
    RecalculateDeliverableEvents(pWin);
    return Success;
}
