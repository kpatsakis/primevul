ProcAllocNamedColor(ClientPtr client)
{
    ColormapPtr pcmp;
    int rc;

    REQUEST(xAllocNamedColorReq);

    REQUEST_FIXED_SIZE(xAllocNamedColorReq, stuff->nbytes);
    rc = dixLookupResourceByType((void **) &pcmp, stuff->cmap, RT_COLORMAP,
                                 client, DixAddAccess);
    if (rc == Success) {
        xAllocNamedColorReply ancr = {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = 0
        };
        if (OsLookupColor
            (pcmp->pScreen->myNum, (char *) &stuff[1], stuff->nbytes,
             &ancr.exactRed, &ancr.exactGreen, &ancr.exactBlue)) {
            ancr.screenRed = ancr.exactRed;
            ancr.screenGreen = ancr.exactGreen;
            ancr.screenBlue = ancr.exactBlue;
            ancr.pixel = 0;
            if ((rc = AllocColor(pcmp,
                                 &ancr.screenRed, &ancr.screenGreen,
                                 &ancr.screenBlue, &ancr.pixel, client->index)))
                return rc;
#ifdef PANORAMIX
            if (noPanoramiXExtension || !pcmp->pScreen->myNum)
#endif
                WriteReplyToClient(client, sizeof(xAllocNamedColorReply),
                                   &ancr);
            return Success;
        }
        else
            return BadName;

    }
    else {
        client->errorValue = stuff->cmap;
        return rc;
    }
}
