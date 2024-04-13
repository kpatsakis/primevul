ProcPolyFillRectangle(ClientPtr client)
{
    int things;
    GC *pGC;
    DrawablePtr pDraw;

    REQUEST(xPolyFillRectangleReq);

    REQUEST_AT_LEAST_SIZE(xPolyFillRectangleReq);
    VALIDATE_DRAWABLE_AND_GC(stuff->drawable, pDraw, DixWriteAccess);
    things = (client->req_len << 2) - sizeof(xPolyFillRectangleReq);
    if (things & 4)
        return BadLength;
    things >>= 3;

    if (things)
        (*pGC->ops->PolyFillRect) (pDraw, pGC, things,
                                   (xRectangle *) &stuff[1]);
    return Success;
}
