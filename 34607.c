ProcDestroySubwindows(ClientPtr client)
{
    WindowPtr pWin;

    REQUEST(xResourceReq);
    int rc;

    REQUEST_SIZE_MATCH(xResourceReq);
    rc = dixLookupWindow(&pWin, stuff->id, client, DixRemoveAccess);
    if (rc != Success)
        return rc;
    DestroySubwindows(pWin, client);
    return Success;
}
