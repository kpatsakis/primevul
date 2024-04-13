ProcCreateWindow(ClientPtr client)
{
    WindowPtr pParent, pWin;

    REQUEST(xCreateWindowReq);
    int len, rc;

    REQUEST_AT_LEAST_SIZE(xCreateWindowReq);

    LEGAL_NEW_RESOURCE(stuff->wid, client);
    rc = dixLookupWindow(&pParent, stuff->parent, client, DixAddAccess);
    if (rc != Success)
        return rc;
    len = client->req_len - bytes_to_int32(sizeof(xCreateWindowReq));
    if (Ones(stuff->mask) != len)
        return BadLength;
    if (!stuff->width || !stuff->height) {
        client->errorValue = 0;
        return BadValue;
    }
    pWin = CreateWindow(stuff->wid, pParent, stuff->x,
                        stuff->y, stuff->width, stuff->height,
                        stuff->borderWidth, stuff->class,
                        stuff->mask, (XID *) &stuff[1],
                        (int) stuff->depth, client, stuff->visual, &rc);
    if (pWin) {
        Mask mask = pWin->eventMask;

        pWin->eventMask = 0;    /* subterfuge in case AddResource fails */
        if (!AddResource(stuff->wid, RT_WINDOW, (void *) pWin))
            return BadAlloc;
        pWin->eventMask = mask;
    }
    return rc;
}
