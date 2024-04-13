ProcCreateGC(ClientPtr client)
{
    int error, rc;
    GC *pGC;
    DrawablePtr pDraw;
    unsigned len;

    REQUEST(xCreateGCReq);

    REQUEST_AT_LEAST_SIZE(xCreateGCReq);
    client->errorValue = stuff->gc;
    LEGAL_NEW_RESOURCE(stuff->gc, client);
    rc = dixLookupDrawable(&pDraw, stuff->drawable, client, 0,
                           DixGetAttrAccess);
    if (rc != Success)
        return rc;

    len = client->req_len - bytes_to_int32(sizeof(xCreateGCReq));
    if (len != Ones(stuff->mask))
        return BadLength;
    pGC = (GC *) CreateGC(pDraw, stuff->mask, (XID *) &stuff[1], &error,
                          stuff->gc, client);
    if (error != Success)
        return error;
    if (!AddResource(stuff->gc, RT_GC, (void *) pGC))
        return BadAlloc;
    return Success;
}
