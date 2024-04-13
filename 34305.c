ProcRenderComposite (ClientPtr client)
{
    PicturePtr	pSrc, pMask, pDst;
    REQUEST(xRenderCompositeReq);

    REQUEST_SIZE_MATCH(xRenderCompositeReq);
    if (!PictOpValid (stuff->op))
    {
	client->errorValue = stuff->op;
	return BadValue;
    }
    VERIFY_PICTURE (pDst, stuff->dst, client, DixWriteAccess);
    if (!pDst->pDrawable)
        return BadDrawable;
    VERIFY_PICTURE (pSrc, stuff->src, client, DixReadAccess);
    VERIFY_ALPHA (pMask, stuff->mask, client, DixReadAccess);
    if ((pSrc->pDrawable && pSrc->pDrawable->pScreen != pDst->pDrawable->pScreen) ||
	(pMask && pMask->pDrawable && pDst->pDrawable->pScreen != pMask->pDrawable->pScreen))
	return BadMatch;
    CompositePicture (stuff->op,
		      pSrc,
		      pMask,
		      pDst,
		      stuff->xSrc,
		      stuff->ySrc,
		      stuff->xMask,
		      stuff->yMask,
		      stuff->xDst,
		      stuff->yDst,
		      stuff->width,
		      stuff->height);
    return Success;
}
