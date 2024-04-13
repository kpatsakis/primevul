ProcDestroyWindow(ClientPtr client)
{
    WindowPtr pWin;

    REQUEST(xResourceReq);
    int rc;

    REQUEST_SIZE_MATCH(xResourceReq);
    rc = dixLookupWindow(&pWin, stuff->id, client, DixDestroyAccess);
    if (rc != Success)
        return rc;
    if (pWin->parent) {
        rc = dixLookupWindow(&pWin, pWin->parent->drawable.id, client,
                             DixRemoveAccess);
        if (rc != Success)
            return rc;
        FreeResource(stuff->id, RT_NONE);
    }
    return Success;
}
