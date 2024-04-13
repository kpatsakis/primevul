ProcFreeColors(ClientPtr client)
{
    ColormapPtr pcmp;
    int rc;

    REQUEST(xFreeColorsReq);

    REQUEST_AT_LEAST_SIZE(xFreeColorsReq);
    rc = dixLookupResourceByType((void **) &pcmp, stuff->cmap, RT_COLORMAP,
                                 client, DixRemoveAccess);
    if (rc == Success) {
        int count;

        if (pcmp->flags & AllAllocated)
            return BadAccess;
        count = bytes_to_int32((client->req_len << 2) - sizeof(xFreeColorsReq));
        return FreeColors(pcmp, client->index, count,
                          (Pixel *) &stuff[1], (Pixel) stuff->planeMask);
    }
    else {
        client->errorValue = stuff->cmap;
        return rc;
    }
}
