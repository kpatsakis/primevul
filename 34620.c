ProcGetWindowAttributes(ClientPtr client)
{
    WindowPtr pWin;

    REQUEST(xResourceReq);
    xGetWindowAttributesReply wa;
    int rc;

    REQUEST_SIZE_MATCH(xResourceReq);
    rc = dixLookupWindow(&pWin, stuff->id, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;
    memset(&wa, 0, sizeof(xGetWindowAttributesReply));
    GetWindowAttributes(pWin, client, &wa);
    WriteReplyToClient(client, sizeof(xGetWindowAttributesReply), &wa);
    return Success;
}
