static int ProcRenderCreateSolidFill(ClientPtr client)
{
    PicturePtr	    pPicture;
    int		    error = 0;
    REQUEST(xRenderCreateSolidFillReq);

    REQUEST_AT_LEAST_SIZE(xRenderCreateSolidFillReq);

    LEGAL_NEW_RESOURCE(stuff->pid, client);

    pPicture = CreateSolidPicture(stuff->pid, &stuff->color, &error);
    if (!pPicture)
	return error;
    /* security creation/labeling check */
    error = XaceHook(XACE_RESOURCE_ACCESS, client, stuff->pid, PictureType,
		     pPicture, RT_NONE, NULL, DixCreateAccess);
    if (error != Success)
	return error;
    if (!AddResource (stuff->pid, PictureType, (pointer)pPicture))
	return BadAlloc;
    return Success;
}
