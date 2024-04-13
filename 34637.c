ProcPolyLine(ClientPtr client)
{
    int npoint;
    GC *pGC;
    DrawablePtr pDraw;

    REQUEST(xPolyLineReq);

    REQUEST_AT_LEAST_SIZE(xPolyLineReq);
    if ((stuff->coordMode != CoordModeOrigin) &&
        (stuff->coordMode != CoordModePrevious)) {
        client->errorValue = stuff->coordMode;
        return BadValue;
    }
    VALIDATE_DRAWABLE_AND_GC(stuff->drawable, pDraw, DixWriteAccess);
    npoint = bytes_to_int32((client->req_len << 2) - sizeof(xPolyLineReq));
    if (npoint > 1)
        (*pGC->ops->Polylines) (pDraw, pGC, stuff->coordMode, npoint,
                                (DDXPointPtr) &stuff[1]);
    return Success;
}
