ProcAllocColorPlanes(ClientPtr client)
{
    ColormapPtr pcmp;
    int rc;

    REQUEST(xAllocColorPlanesReq);

    REQUEST_SIZE_MATCH(xAllocColorPlanesReq);
    rc = dixLookupResourceByType((void **) &pcmp, stuff->cmap, RT_COLORMAP,
                                 client, DixAddAccess);
    if (rc == Success) {
        xAllocColorPlanesReply acpr;
        int npixels;
        long length;
        Pixel *ppixels;

        npixels = stuff->colors;
        if (!npixels) {
            client->errorValue = npixels;
            return BadValue;
        }
        if (stuff->contiguous != xTrue && stuff->contiguous != xFalse) {
            client->errorValue = stuff->contiguous;
            return BadValue;
        }
        acpr = (xAllocColorPlanesReply) {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .nPixels = npixels
        };
        length = (long) npixels *sizeof(Pixel);

        ppixels = malloc(length);
        if (!ppixels)
            return BadAlloc;
        if ((rc = AllocColorPlanes(client->index, pcmp, npixels,
                                   (int) stuff->red, (int) stuff->green,
                                   (int) stuff->blue, (Bool) stuff->contiguous,
                                   ppixels, &acpr.redMask, &acpr.greenMask,
                                   &acpr.blueMask))) {
            free(ppixels);
            return rc;
        }
        acpr.length = bytes_to_int32(length);
#ifdef PANORAMIX
        if (noPanoramiXExtension || !pcmp->pScreen->myNum)
#endif
        {
            WriteReplyToClient(client, sizeof(xAllocColorPlanesReply), &acpr);
            client->pSwapReplyFunc = (ReplySwapPtr) Swap32Write;
            WriteSwappedDataToClient(client, length, ppixels);
        }
        free(ppixels);
        return Success;
    }
    else {
        client->errorValue = stuff->cmap;
        return rc;
    }
}
