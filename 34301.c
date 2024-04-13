ProcRenderAddTraps (ClientPtr client)
{
    int		ntraps;
    PicturePtr	pPicture;
    REQUEST(xRenderAddTrapsReq);

    REQUEST_AT_LEAST_SIZE(xRenderAddTrapsReq);
    VERIFY_PICTURE (pPicture, stuff->picture, client, DixWriteAccess);
    if (!pPicture->pDrawable)
        return BadDrawable;
    ntraps = (client->req_len << 2) - sizeof (xRenderAddTrapsReq);
    if (ntraps % sizeof (xTrap))
	return BadLength;
    ntraps /= sizeof (xTrap);
    if (ntraps)
	AddTraps (pPicture,
		  stuff->xOff, stuff->yOff,
		  ntraps, (xTrap *) &stuff[1]);
    return Success;
}
