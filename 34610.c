ProcFillPoly(ClientPtr client)
{
    int things;
    GC *pGC;
    DrawablePtr pDraw;

    REQUEST(xFillPolyReq);

    REQUEST_AT_LEAST_SIZE(xFillPolyReq);
    if ((stuff->shape != Complex) && (stuff->shape != Nonconvex) &&
        (stuff->shape != Convex)) {
        client->errorValue = stuff->shape;
        return BadValue;
    }
    if ((stuff->coordMode != CoordModeOrigin) &&
        (stuff->coordMode != CoordModePrevious)) {
        client->errorValue = stuff->coordMode;
        return BadValue;
    }

    VALIDATE_DRAWABLE_AND_GC(stuff->drawable, pDraw, DixWriteAccess);
    things = bytes_to_int32((client->req_len << 2) - sizeof(xFillPolyReq));
    if (things)
        (*pGC->ops->FillPolygon) (pDraw, pGC, stuff->shape,
                                  stuff->coordMode, things,
                                  (DDXPointPtr) &stuff[1]);
    return Success;
}
