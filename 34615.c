ProcFreePixmap(ClientPtr client)
{
    PixmapPtr pMap;
    int rc;

    REQUEST(xResourceReq);
    REQUEST_SIZE_MATCH(xResourceReq);

    rc = dixLookupResourceByType((void **) &pMap, stuff->id, RT_PIXMAP,
                                 client, DixDestroyAccess);
    if (rc == Success) {
        FreeResource(stuff->id, RT_NONE);
        return Success;
    }
    else {
        client->errorValue = stuff->id;
        return rc;
    }
}
