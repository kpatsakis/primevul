ProcAllocColor(ClientPtr client)
{
    ColormapPtr pmap;
    int rc;

    REQUEST(xAllocColorReq);

    REQUEST_SIZE_MATCH(xAllocColorReq);
    rc = dixLookupResourceByType((void **) &pmap, stuff->cmap, RT_COLORMAP,
                                 client, DixAddAccess);
    if (rc == Success) {
        xAllocColorReply acr = {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = 0,
            .red = stuff->red,
            .green = stuff->green,
            .blue = stuff->blue,
            .pixel = 0
        };
        if ((rc = AllocColor(pmap, &acr.red, &acr.green, &acr.blue,
                             &acr.pixel, client->index)))
            return rc;
#ifdef PANORAMIX
        if (noPanoramiXExtension || !pmap->pScreen->myNum)
#endif
            WriteReplyToClient(client, sizeof(xAllocColorReply), &acr);
        return Success;

    }
    else {
        client->errorValue = stuff->cmap;
        return rc;
    }
}
