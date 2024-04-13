ProcCreateCursor(ClientPtr client)
{
    CursorPtr pCursor;
    PixmapPtr src;
    PixmapPtr msk;
    unsigned char *srcbits;
    unsigned char *mskbits;
    unsigned short width, height;
    long n;
    CursorMetricRec cm;
    int rc;

    REQUEST(xCreateCursorReq);

    REQUEST_SIZE_MATCH(xCreateCursorReq);
    LEGAL_NEW_RESOURCE(stuff->cid, client);

    rc = dixLookupResourceByType((void **) &src, stuff->source, RT_PIXMAP,
                                 client, DixReadAccess);
    if (rc != Success) {
        client->errorValue = stuff->source;
        return rc;
    }

    if (src->drawable.depth != 1)
        return (BadMatch);

    /* Find and validate cursor mask pixmap, if one is provided */
    if (stuff->mask != None) {
        rc = dixLookupResourceByType((void **) &msk, stuff->mask, RT_PIXMAP,
                                     client, DixReadAccess);
        if (rc != Success) {
            client->errorValue = stuff->mask;
            return rc;
        }

        if (src->drawable.width != msk->drawable.width
            || src->drawable.height != msk->drawable.height
            || src->drawable.depth != 1 || msk->drawable.depth != 1)
            return BadMatch;
    }
    else
        msk = NULL;

    width = src->drawable.width;
    height = src->drawable.height;

    if (stuff->x > width || stuff->y > height)
        return BadMatch;

    srcbits = calloc(BitmapBytePad(width), height);
    if (!srcbits)
        return BadAlloc;
    n = BitmapBytePad(width) * height;
    mskbits = malloc(n);
    if (!mskbits) {
        free(srcbits);
        return BadAlloc;
    }

    (*src->drawable.pScreen->GetImage) ((DrawablePtr) src, 0, 0, width, height,
                                        XYPixmap, 1, (void *) srcbits);
    if (msk == (PixmapPtr) NULL) {
        unsigned char *bits = mskbits;

        while (--n >= 0)
            *bits++ = ~0;
    }
    else {
        /* zeroing the (pad) bits helps some ddx cursor handling */
        memset((char *) mskbits, 0, n);
        (*msk->drawable.pScreen->GetImage) ((DrawablePtr) msk, 0, 0, width,
                                            height, XYPixmap, 1,
                                            (void *) mskbits);
    }
    cm.width = width;
    cm.height = height;
    cm.xhot = stuff->x;
    cm.yhot = stuff->y;
    rc = AllocARGBCursor(srcbits, mskbits, NULL, &cm,
                         stuff->foreRed, stuff->foreGreen, stuff->foreBlue,
                         stuff->backRed, stuff->backGreen, stuff->backBlue,
                         &pCursor, client, stuff->cid);

    if (rc != Success)
        goto bail;
    if (!AddResource(stuff->cid, RT_CURSOR, (void *) pCursor)) {
        rc = BadAlloc;
        goto bail;
    }

    return Success;
 bail:
    free(srcbits);
    free(mskbits);
    return rc;
}
