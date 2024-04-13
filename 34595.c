ProcCloseFont(ClientPtr client)
{
    FontPtr pFont;
    int rc;

    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    rc = dixLookupResourceByType((void **) &pFont, stuff->id, RT_FONT,
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
