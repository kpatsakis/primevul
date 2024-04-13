ProcChangeSaveSet(ClientPtr client)
{
    WindowPtr pWin;

    REQUEST(xChangeSaveSetReq);
    int rc;

    REQUEST_SIZE_MATCH(xChangeSaveSetReq);
    rc = dixLookupWindow(&pWin, stuff->window, client, DixManageAccess);
    if (rc != Success)
        return rc;
    if (client->clientAsMask == (CLIENT_BITS(pWin->drawable.id)))
        return BadMatch;
    if ((stuff->mode == SetModeInsert) || (stuff->mode == SetModeDelete))
        return AlterSaveSetForClient(client, pWin, stuff->mode, FALSE, TRUE);
    client->errorValue = stuff->mode;
    return BadValue;
}
