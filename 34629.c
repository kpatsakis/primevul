ProcLookupColor(ClientPtr client)
{
    ColormapPtr pcmp;
    int rc;

    REQUEST(xLookupColorReq);

    REQUEST_FIXED_SIZE(xLookupColorReq, stuff->nbytes);
    rc = dixLookupResourceByType((void **) &pcmp, stuff->cmap, RT_COLORMAP,
                                 client, DixReadAccess);
    if (rc == Success) {
        CARD16 exactRed, exactGreen, exactBlue;

        if (OsLookupColor
            (pcmp->pScreen->myNum, (char *) &stuff[1], stuff->nbytes,
             &exactRed, &exactGreen, &exactBlue)) {
            xLookupColorReply lcr = {
                .type = X_Reply,
                .sequenceNumber = client->sequence,
                .length = 0,
                .exactRed = exactRed,
                .exactGreen = exactGreen,
                .exactBlue = exactBlue,
                .screenRed = exactRed,
                .screenGreen = exactGreen,
                .screenBlue = exactBlue
            };
            (*pcmp->pScreen->ResolveColor) (&lcr.screenRed,
                                            &lcr.screenGreen,
                                            &lcr.screenBlue, pcmp->pVisual);
            WriteReplyToClient(client, sizeof(xLookupColorReply), &lcr);
            return Success;
        }
        return BadName;
    }
    else {
        client->errorValue = stuff->cmap;
        return rc;
    }
}
