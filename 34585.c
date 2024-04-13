ProcAllocColorCells(ClientPtr client)
{
    ColormapPtr pcmp;
    int rc;

    REQUEST(xAllocColorCellsReq);

    REQUEST_SIZE_MATCH(xAllocColorCellsReq);
    rc = dixLookupResourceByType((void **) &pcmp, stuff->cmap, RT_COLORMAP,
                                 client, DixAddAccess);
    if (rc == Success) {
        int npixels, nmasks;
        long length;
        Pixel *ppixels, *pmasks;

        npixels = stuff->colors;
        if (!npixels) {
            client->errorValue = npixels;
            return BadValue;
        }
        if (stuff->contiguous != xTrue && stuff->contiguous != xFalse) {
            client->errorValue = stuff->contiguous;
            return BadValue;
        }
        nmasks = stuff->planes;
        length = ((long) npixels + (long) nmasks) * sizeof(Pixel);
        ppixels = malloc(length);
        if (!ppixels)
            return BadAlloc;
        pmasks = ppixels + npixels;

        if ((rc = AllocColorCells(client->index, pcmp, npixels, nmasks,
                                  (Bool) stuff->contiguous, ppixels, pmasks))) {
            free(ppixels);
            return rc;
        }
#ifdef PANORAMIX
        if (noPanoramiXExtension || !pcmp->pScreen->myNum)
#endif
        {
            xAllocColorCellsReply accr = {
                .type = X_Reply,
                .sequenceNumber = client->sequence,
                .length = bytes_to_int32(length),
                .nPixels = npixels,
                .nMasks = nmasks
            };
            WriteReplyToClient(client, sizeof(xAllocColorCellsReply), &accr);
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
