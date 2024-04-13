ProcStoreNamedColor(ClientPtr client)
{
    ColormapPtr pcmp;
    int rc;

    REQUEST(xStoreNamedColorReq);

    REQUEST_FIXED_SIZE(xStoreNamedColorReq, stuff->nbytes);
    rc = dixLookupResourceByType((void **) &pcmp, stuff->cmap, RT_COLORMAP,
                                 client, DixWriteAccess);
    if (rc == Success) {
        xColorItem def;

        if (OsLookupColor(pcmp->pScreen->myNum, (char *) &stuff[1],
                          stuff->nbytes, &def.red, &def.green, &def.blue)) {
            def.flags = stuff->flags;
            def.pixel = stuff->pixel;
            return StoreColors(pcmp, 1, &def, client);
        }
        return BadName;
    }
    else {
        client->errorValue = stuff->cmap;
        return rc;
    }
}
