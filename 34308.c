static int ProcRenderCreateConicalGradient (ClientPtr client)
{
    PicturePtr	    pPicture;
    int		    len;
    int		    error = 0;
    xFixed          *stops;
    xRenderColor   *colors;
    REQUEST(xRenderCreateConicalGradientReq);

    REQUEST_AT_LEAST_SIZE(xRenderCreateConicalGradientReq);

    LEGAL_NEW_RESOURCE(stuff->pid, client);

    len = (client->req_len << 2) - sizeof(xRenderCreateConicalGradientReq);
    if (len != stuff->nStops*(sizeof(xFixed) + sizeof(xRenderColor)))
        return BadLength;

    stops = (xFixed *)(stuff + 1);
    colors = (xRenderColor *)(stops + stuff->nStops);

    pPicture = CreateConicalGradientPicture (stuff->pid, &stuff->center, stuff->angle,
                                             stuff->nStops, stops, colors, &error);
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
