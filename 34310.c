static int ProcRenderCreateLinearGradient (ClientPtr client)
{
    PicturePtr	    pPicture;
    int		    len;
    int		    error = 0;
    xFixed          *stops;
    xRenderColor   *colors;
    REQUEST(xRenderCreateLinearGradientReq);

    REQUEST_AT_LEAST_SIZE(xRenderCreateLinearGradientReq);

    LEGAL_NEW_RESOURCE(stuff->pid, client);

    len = (client->req_len << 2) - sizeof(xRenderCreateLinearGradientReq);
    if (stuff->nStops > UINT32_MAX/(sizeof(xFixed) + sizeof(xRenderColor)))
	return BadLength;
    if (len != stuff->nStops*(sizeof(xFixed) + sizeof(xRenderColor)))
        return BadLength;

    stops = (xFixed *)(stuff + 1);
    colors = (xRenderColor *)(stops + stuff->nStops);

    pPicture = CreateLinearGradientPicture (stuff->pid, &stuff->p1, &stuff->p2,
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
