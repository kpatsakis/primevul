ProcRenderTrapezoids (ClientPtr client)
{
    int		rc, ntraps;
    PicturePtr	pSrc, pDst;
    PictFormatPtr   pFormat;
    REQUEST(xRenderTrapezoidsReq);

    REQUEST_AT_LEAST_SIZE(xRenderTrapezoidsReq);
    if (!PictOpValid (stuff->op))
    {
	client->errorValue = stuff->op;
	return BadValue;
    }
    VERIFY_PICTURE (pSrc, stuff->src, client, DixReadAccess);
    VERIFY_PICTURE (pDst, stuff->dst, client, DixWriteAccess);
    if (!pDst->pDrawable)
        return BadDrawable;
    if (pSrc->pDrawable && pSrc->pDrawable->pScreen != pDst->pDrawable->pScreen)
	return BadMatch;
    if (stuff->maskFormat)
    {
	rc = dixLookupResourceByType((pointer *)&pFormat, stuff->maskFormat,
				     PictFormatType, client, DixReadAccess);
	if (rc != Success)
	    return rc;
    }
    else
	pFormat = 0;
    ntraps = (client->req_len << 2) - sizeof (xRenderTrapezoidsReq);
    if (ntraps % sizeof (xTrapezoid))
	return BadLength;
    ntraps /= sizeof (xTrapezoid);
    if (ntraps)
	CompositeTrapezoids (stuff->op, pSrc, pDst, pFormat,
			     stuff->xSrc, stuff->ySrc,
			     ntraps, (xTrapezoid *) &stuff[1]);
    return Success;
}
