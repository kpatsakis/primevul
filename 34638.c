ProcPolyPoint(ClientPtr client)
{
    int npoint;
    GC *pGC;
    DrawablePtr pDraw;

    REQUEST(xPolyPointReq);

    REQUEST_AT_LEAST_SIZE(xPolyPointReq);
    if ((stuff->coordMode != CoordModeOrigin) &&
        (stuff->coordMode != CoordModePrevious)) {
        client->errorValue = stuff->coordMode;
        return BadValue;
    }
    VALIDATE_DRAWABLE_AND_GC(stuff->drawable, pDraw, DixWriteAccess);
    npoint = bytes_to_int32((client->req_len << 2) - sizeof(xPolyPointReq));
    if (npoint)
        (*pGC->ops->PolyPoint) (pDraw, pGC, stuff->coordMode, npoint,
                                (xPoint *) &stuff[1]);
    return Success;
}
