ProcChangeGC(ClientPtr client)
{
    GC *pGC;
    int result;
    unsigned len;

    REQUEST(xChangeGCReq);
    REQUEST_AT_LEAST_SIZE(xChangeGCReq);

    result = dixLookupGC(&pGC, stuff->gc, client, DixSetAttrAccess);
    if (result != Success)
        return result;

    len = client->req_len - bytes_to_int32(sizeof(xChangeGCReq));
    if (len != Ones(stuff->mask))
        return BadLength;

    return ChangeGCXIDs(client, pGC, stuff->mask, (CARD32 *) &stuff[1]);
}
