ProcCopyArea(ClientPtr client)
{
    DrawablePtr pDst;
    DrawablePtr pSrc;
    GC *pGC;

    REQUEST(xCopyAreaReq);
    RegionPtr pRgn;
    int rc;

    REQUEST_SIZE_MATCH(xCopyAreaReq);

    VALIDATE_DRAWABLE_AND_GC(stuff->dstDrawable, pDst, DixWriteAccess);
    if (stuff->dstDrawable != stuff->srcDrawable) {
        rc = dixLookupDrawable(&pSrc, stuff->srcDrawable, client, 0,
                               DixReadAccess);
        if (rc != Success)
            return rc;
        if ((pDst->pScreen != pSrc->pScreen) || (pDst->depth != pSrc->depth)) {
            client->errorValue = stuff->dstDrawable;
            return BadMatch;
        }
    }
    else
        pSrc = pDst;

    pRgn = (*pGC->ops->CopyArea) (pSrc, pDst, pGC, stuff->srcX, stuff->srcY,
                                  stuff->width, stuff->height,
                                  stuff->dstX, stuff->dstY);
    if (pGC->graphicsExposures) {
        SendGraphicsExpose(client, pRgn, stuff->dstDrawable, X_CopyArea, 0);
        if (pRgn)
            RegionDestroy(pRgn);
    }

    return Success;
}
