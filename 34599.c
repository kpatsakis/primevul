ProcCopyGC(ClientPtr client)
{
    GC *dstGC;
    GC *pGC;
    int result;

    REQUEST(xCopyGCReq);
    REQUEST_SIZE_MATCH(xCopyGCReq);

    result = dixLookupGC(&pGC, stuff->srcGC, client, DixGetAttrAccess);
    if (result != Success)
        return result;
    result = dixLookupGC(&dstGC, stuff->dstGC, client, DixSetAttrAccess);
    if (result != Success)
        return result;
    if ((dstGC->pScreen != pGC->pScreen) || (dstGC->depth != pGC->depth))
        return BadMatch;
    if (stuff->mask & ~GCAllBits) {
        client->errorValue = stuff->mask;
        return BadValue;
    }
    return CopyGC(pGC, dstGC, stuff->mask);
}
