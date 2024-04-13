DoGetImage(ClientPtr client, int format, Drawable drawable,
           int x, int y, int width, int height,
           Mask planemask)
{
    DrawablePtr pDraw, pBoundingDraw;
    int nlines, linesPerBuf, rc;
    int linesDone;

    /* coordinates relative to the bounding drawable */
    int relx, rely;
    long widthBytesLine, length;
    Mask plane = 0;
    char *pBuf;
    xGetImageReply xgi;
    RegionPtr pVisibleRegion = NULL;

    if ((format != XYPixmap) && (format != ZPixmap)) {
        client->errorValue = format;
        return BadValue;
    }
    rc = dixLookupDrawable(&pDraw, drawable, client, 0, DixReadAccess);
    if (rc != Success)
        return rc;

    memset(&xgi, 0, sizeof(xGetImageReply));

    relx = x;
    rely = y;

    if (pDraw->type == DRAWABLE_WINDOW) {
        WindowPtr pWin = (WindowPtr) pDraw;

        /* "If the drawable is a window, the window must be viewable ... or a
         * BadMatch error results" */
        if (!pWin->viewable)
            return BadMatch;

        /* If the drawable is a window, the rectangle must be contained within
         * its bounds (including the border). */
        if (x < -wBorderWidth(pWin) ||
            x + width > wBorderWidth(pWin) + (int) pDraw->width ||
            y < -wBorderWidth(pWin) ||
            y + height > wBorderWidth(pWin) + (int) pDraw->height)
            return BadMatch;

        relx += pDraw->x;
        rely += pDraw->y;

        if (pDraw->pScreen->GetWindowPixmap) {
            PixmapPtr pPix = (*pDraw->pScreen->GetWindowPixmap) (pWin);

            pBoundingDraw = &pPix->drawable;
#ifdef COMPOSITE
            relx -= pPix->screen_x;
            rely -= pPix->screen_y;
#endif
        }
        else {
            pBoundingDraw = (DrawablePtr) pDraw->pScreen->root;
        }

        xgi.visual = wVisual(pWin);
    }
    else {
        pBoundingDraw = pDraw;
        xgi.visual = None;
    }

    /* "If the drawable is a pixmap, the given rectangle must be wholly
     *  contained within the pixmap, or a BadMatch error results.  If the
     *  drawable is a window [...] it must be the case that if there were no
     *  inferiors or overlapping windows, the specified rectangle of the window
     *  would be fully visible on the screen and wholly contained within the
     *  outside edges of the window, or a BadMatch error results."
     *
     * We relax the window case slightly to mean that the rectangle must exist
     * within the bounds of the window's backing pixmap.  In particular, this
     * means that a GetImage request may succeed or fail with BadMatch depending
     * on whether any of its ancestor windows are redirected.  */
    if (relx < 0 || relx + width > (int) pBoundingDraw->width ||
        rely < 0 || rely + height > (int) pBoundingDraw->height)
        return BadMatch;

    xgi.type = X_Reply;
    xgi.sequenceNumber = client->sequence;
    xgi.depth = pDraw->depth;
    if (format == ZPixmap) {
        widthBytesLine = PixmapBytePad(width, pDraw->depth);
        length = widthBytesLine * height;

    }
    else {
        widthBytesLine = BitmapBytePad(width);
        plane = ((Mask) 1) << (pDraw->depth - 1);
        /* only planes asked for */
        length = widthBytesLine * height *
            Ones(planemask & (plane | (plane - 1)));

    }

    xgi.length = length;

    xgi.length = bytes_to_int32(xgi.length);
    if (widthBytesLine == 0 || height == 0)
        linesPerBuf = 0;
    else if (widthBytesLine >= IMAGE_BUFSIZE)
        linesPerBuf = 1;
    else {
        linesPerBuf = IMAGE_BUFSIZE / widthBytesLine;
        if (linesPerBuf > height)
            linesPerBuf = height;
    }
    length = linesPerBuf * widthBytesLine;
    if (linesPerBuf < height) {
        /* we have to make sure intermediate buffers don't need padding */
        while ((linesPerBuf > 1) &&
               (length & ((1L << LOG2_BYTES_PER_SCANLINE_PAD) - 1))) {
            linesPerBuf--;
            length -= widthBytesLine;
        }
        while (length & ((1L << LOG2_BYTES_PER_SCANLINE_PAD) - 1)) {
            linesPerBuf++;
            length += widthBytesLine;
        }
    }
    if (!(pBuf = calloc(1, length)))
        return BadAlloc;
    WriteReplyToClient(client, sizeof(xGetImageReply), &xgi);

    if (pDraw->type == DRAWABLE_WINDOW)
        pVisibleRegion = &((WindowPtr) pDraw)->borderClip;

    if (linesPerBuf == 0) {
        /* nothing to do */
    }
    else if (format == ZPixmap) {
        linesDone = 0;
        while (height - linesDone > 0) {
            nlines = min(linesPerBuf, height - linesDone);
            (*pDraw->pScreen->GetImage) (pDraw,
                                         x,
                                         y + linesDone,
                                         width,
                                         nlines,
                                         format, planemask, (void *) pBuf);
            if (pVisibleRegion)
                XaceCensorImage(client, pVisibleRegion, widthBytesLine,
                                pDraw, x, y + linesDone, width,
                                nlines, format, pBuf);

            /* Note that this is NOT a call to WriteSwappedDataToClient,
               as we do NOT byte swap */
            ReformatImage(pBuf, (int) (nlines * widthBytesLine),
                          BitsPerPixel(pDraw->depth), ClientOrder(client));

            WriteToClient(client, (int) (nlines * widthBytesLine), pBuf);
            linesDone += nlines;
        }
    }
    else {                      /* XYPixmap */

        for (; plane; plane >>= 1) {
            if (planemask & plane) {
                linesDone = 0;
                while (height - linesDone > 0) {
                    nlines = min(linesPerBuf, height - linesDone);
                    (*pDraw->pScreen->GetImage) (pDraw,
                                                 x,
                                                 y + linesDone,
                                                 width,
                                                 nlines,
                                                 format, plane, (void *) pBuf);
                    if (pVisibleRegion)
                        XaceCensorImage(client, pVisibleRegion,
                                        widthBytesLine,
                                        pDraw, x, y + linesDone, width,
                                        nlines, format, pBuf);

                    /* Note: NOT a call to WriteSwappedDataToClient,
                       as we do NOT byte swap */
                    ReformatImage(pBuf, (int) (nlines * widthBytesLine),
                                  1, ClientOrder(client));

                    WriteToClient(client, (int)(nlines * widthBytesLine), pBuf);
                    linesDone += nlines;
                }
            }
        }
    }
    free(pBuf);
    return Success;
}
