ProcInstallColormap(ClientPtr client)
{
    ColormapPtr pcmp;
    int rc;

    REQUEST(xResourceReq);
    REQUEST_SIZE_MATCH(xResourceReq);

    rc = dixLookupResourceByType((void **) &pcmp, stuff->id, RT_COLORMAP,
                                 client, DixInstallAccess);
    if (rc != Success)
        goto out;

    rc = XaceHook(XACE_SCREEN_ACCESS, client, pcmp->pScreen, DixSetAttrAccess);
    if (rc != Success) {
        if (rc == BadValue)
            rc = BadColor;
        goto out;
    }

    (*(pcmp->pScreen->InstallColormap)) (pcmp);
    return Success;

 out:
    client->errorValue = stuff->id;
    return rc;
}
