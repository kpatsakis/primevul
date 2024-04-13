ProcPolyArc(ClientPtr client)
{
    int narcs;
    GC *pGC;
    DrawablePtr pDraw;

    REQUEST(xPolyArcReq);

    REQUEST_AT_LEAST_SIZE(xPolyArcReq);
    VALIDATE_DRAWABLE_AND_GC(stuff->drawable, pDraw, DixWriteAccess);
    narcs = (client->req_len << 2) - sizeof(xPolyArcReq);
    if (narcs % sizeof(xArc))
        return BadLength;
    narcs /= sizeof(xArc);
    if (narcs)
        (*pGC->ops->PolyArc) (pDraw, pGC, narcs, (xArc *) &stuff[1]);
    return Success;
}
