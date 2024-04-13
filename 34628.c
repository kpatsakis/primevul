ProcListInstalledColormaps(ClientPtr client)
{
    xListInstalledColormapsReply *preply;
    int nummaps, rc;
    WindowPtr pWin;

    REQUEST(xResourceReq);
    REQUEST_SIZE_MATCH(xResourceReq);

    rc = dixLookupWindow(&pWin, stuff->id, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    rc = XaceHook(XACE_SCREEN_ACCESS, client, pWin->drawable.pScreen,
                  DixGetAttrAccess);
    if (rc != Success)
        return rc;

    preply = malloc(sizeof(xListInstalledColormapsReply) +
                    pWin->drawable.pScreen->maxInstalledCmaps *
                    sizeof(Colormap));
    if (!preply)
        return BadAlloc;

    preply->type = X_Reply;
    preply->sequenceNumber = client->sequence;
    nummaps = (*pWin->drawable.pScreen->ListInstalledColormaps)
        (pWin->drawable.pScreen, (Colormap *) &preply[1]);
    preply->nColormaps = nummaps;
    preply->length = nummaps;
    WriteReplyToClient(client, sizeof(xListInstalledColormapsReply), preply);
    client->pSwapReplyFunc = (ReplySwapPtr) Swap32Write;
    WriteSwappedDataToClient(client, nummaps * sizeof(Colormap), &preply[1]);
    free(preply);
    return Success;
}
