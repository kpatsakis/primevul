ProcPolyText(ClientPtr client)
{
    int err;

    REQUEST(xPolyTextReq);
    DrawablePtr pDraw;
    GC *pGC;

    REQUEST_AT_LEAST_SIZE(xPolyTextReq);
    VALIDATE_DRAWABLE_AND_GC(stuff->drawable, pDraw, DixWriteAccess);

    err = PolyText(client,
                   pDraw,
                   pGC,
                   (unsigned char *) &stuff[1],
                   ((unsigned char *) stuff) + (client->req_len << 2),
                   stuff->x, stuff->y, stuff->reqType, stuff->drawable);

    if (err == Success) {
        return Success;
    }
    else
        return err;
}
