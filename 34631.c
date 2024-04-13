ProcMapWindow(ClientPtr client)
{
    WindowPtr pWin;

    REQUEST(xResourceReq);
    int rc;

    REQUEST_SIZE_MATCH(xResourceReq);
    rc = dixLookupWindow(&pWin, stuff->id, client, DixShowAccess);
    if (rc != Success)
        return rc;
    MapWindow(pWin, client);
    /* update cache to say it is mapped */
    return Success;
}
