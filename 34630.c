ProcMapSubwindows(ClientPtr client)
{
    WindowPtr pWin;

    REQUEST(xResourceReq);
    int rc;

    REQUEST_SIZE_MATCH(xResourceReq);
    rc = dixLookupWindow(&pWin, stuff->id, client, DixListAccess);
    if (rc != Success)
        return rc;
    MapSubwindows(pWin, client);
    /* update cache to say it is mapped */
    return Success;
}
