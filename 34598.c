ProcCopyColormapAndFree(ClientPtr client)
{
    Colormap mid;
    ColormapPtr pSrcMap;

    REQUEST(xCopyColormapAndFreeReq);
    int rc;

    REQUEST_SIZE_MATCH(xCopyColormapAndFreeReq);
    mid = stuff->mid;
    LEGAL_NEW_RESOURCE(mid, client);
    rc = dixLookupResourceByType((void **) &pSrcMap, stuff->srcCmap,
                                 RT_COLORMAP, client,
                                 DixReadAccess | DixRemoveAccess);
    if (rc == Success)
        return CopyColormapAndFree(mid, pSrcMap, client->index);
    client->errorValue = stuff->srcCmap;
    return rc;
}
