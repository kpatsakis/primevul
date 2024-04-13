ProcDbeDeallocateBackBufferName(ClientPtr client)
{
    REQUEST(xDbeDeallocateBackBufferNameReq);
    DbeWindowPrivPtr pDbeWindowPriv;
    int rc, i;
    void *val;

    REQUEST_SIZE_MATCH(xDbeDeallocateBackBufferNameReq);

    /* Buffer name must be valid */
    rc = dixLookupResourceByType((void **) &pDbeWindowPriv, stuff->buffer,
                                 dbeWindowPrivResType, client,
                                 DixDestroyAccess);
    if (rc != Success)
        return rc;

    rc = dixLookupResourceByType(&val, stuff->buffer, dbeDrawableResType,
                                 client, DixDestroyAccess);
    if (rc != Success)
        return rc;

    /* Make sure that the id is valid for the window.
     * This is paranoid code since we already looked up the ID by type
     * above.
     */

    for (i = 0; i < pDbeWindowPriv->nBufferIDs; i++) {
        /* Loop through the ID list to find the ID. */
        if (pDbeWindowPriv->IDs[i] == stuff->buffer) {
            break;
        }
    }

    if (i == pDbeWindowPriv->nBufferIDs) {
        /* We did not find the ID in the ID list. */
        client->errorValue = stuff->buffer;
        return dbeErrorBase + DbeBadBuffer;
    }

    FreeResource(stuff->buffer, RT_NONE);

    return Success;

}                               /* ProcDbeDeallocateBackBufferName() */
