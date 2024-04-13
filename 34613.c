ProcFreeCursor(ClientPtr client)
{
    CursorPtr pCursor;
    int rc;

    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    rc = dixLookupResourceByType((void **) &pCursor, stuff->id, RT_CURSOR,
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
