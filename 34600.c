ProcCopyPlane(ClientPtr client)
{
    DrawablePtr psrcDraw, pdstDraw;
    GC *pGC;

    REQUEST(xCopyPlaneReq);
    RegionPtr pRgn;
    int rc;

    REQUEST_SIZE_MATCH(xCopyPlaneReq);

    VALIDATE_DRAWABLE_AND_GC(stuff->dstDrawable, pdstDraw, DixWriteAccess);
    if (stuff->dstDrawable != stuff->srcDrawable) {
        rc = dixLookupDrawable(&psrcDraw, stuff->srcDrawable, client, 0,
                               DixReadAccess);
        if (rc != Success)
            return rc;

        if (pdstDraw->pScreen != psrcDraw->pScreen) {
            client->errorValue = stuff->dstDrawable;
            return BadMatch;
        }
    }
    else
        psrcDraw = pdstDraw;

    /* Check to see if stuff->bitPlane has exactly ONE good bit set */
    if (stuff->bitPlane == 0 || (stuff->bitPlane & (stuff->bitPlane - 1)) ||
        (stuff->bitPlane > (1L << (psrcDraw->depth - 1)))) {
        client->errorValue = stuff->bitPlane;
        return BadValue;
    }

    pRgn =
        (*pGC->ops->CopyPlane) (psrcDraw, pdstDraw, pGC, stuff->srcX,
                                stuff->srcY, stuff->width, stuff->height,
                                stuff->dstX, stuff->dstY, stuff->bitPlane);
    if (pGC->graphicsExposures) {
        SendGraphicsExpose(client, pRgn, stuff->dstDrawable, X_CopyPlane, 0);
        if (pRgn)
            RegionDestroy(pRgn);
    }
    return Success;
}
