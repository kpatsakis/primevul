ProcRenderTriFan (ClientPtr client)
{
    int		rc, npoints;
    PicturePtr	pSrc, pDst;
    PictFormatPtr   pFormat;
    REQUEST(xRenderTrianglesReq);

    REQUEST_AT_LEAST_SIZE(xRenderTrianglesReq);
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
    npoints = ((client->req_len << 2) - sizeof (xRenderTriStripReq));
    if (npoints & 4)
	return BadLength;
    npoints >>= 3;
    if (npoints >= 3)
	CompositeTriFan (stuff->op, pSrc, pDst, pFormat,
			 stuff->xSrc, stuff->ySrc,
			 npoints, (xPointFixed *) &stuff[1]);
    return Success;
}
