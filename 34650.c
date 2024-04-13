ProcStoreColors(ClientPtr client)
{
    ColormapPtr pcmp;
    int rc;

    REQUEST(xStoreColorsReq);

    REQUEST_AT_LEAST_SIZE(xStoreColorsReq);
    rc = dixLookupResourceByType((void **) &pcmp, stuff->cmap, RT_COLORMAP,
                                 client, DixWriteAccess);
    if (rc == Success) {
        int count;

        count = (client->req_len << 2) - sizeof(xStoreColorsReq);
        if (count % sizeof(xColorItem))
            return BadLength;
        count /= sizeof(xColorItem);
        return StoreColors(pcmp, count, (xColorItem *) &stuff[1], client);
    }
    else {
        client->errorValue = stuff->cmap;
        return rc;
    }
}
