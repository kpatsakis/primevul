ProcImageText16(ClientPtr client)
{
    int err;
    DrawablePtr pDraw;
    GC *pGC;

    REQUEST(xImageTextReq);

    REQUEST_FIXED_SIZE(xImageTextReq, stuff->nChars << 1);
    VALIDATE_DRAWABLE_AND_GC(stuff->drawable, pDraw, DixWriteAccess);

    err = ImageText(client,
                    pDraw,
                    pGC,
                    stuff->nChars,
                    (unsigned char *) &stuff[1],
                    stuff->x, stuff->y, stuff->reqType, stuff->drawable);

    if (err == Success) {
        return Success;
    }
    else
        return err;
}
