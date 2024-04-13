ProcFreeGC(ClientPtr client)
{
    GC *pGC;
    int rc;

    REQUEST(xResourceReq);
    REQUEST_SIZE_MATCH(xResourceReq);

    rc = dixLookupGC(&pGC, stuff->id, client, DixDestroyAccess);
    if (rc != Success)
        return rc;

    FreeResource(stuff->id, RT_NONE);
    return Success;
}
