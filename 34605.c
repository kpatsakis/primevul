ProcCreatePixmap(ClientPtr client)
{
    PixmapPtr pMap;
    DrawablePtr pDraw;

    REQUEST(xCreatePixmapReq);
    DepthPtr pDepth;
    int i, rc;

    REQUEST_SIZE_MATCH(xCreatePixmapReq);
    client->errorValue = stuff->pid;
    LEGAL_NEW_RESOURCE(stuff->pid, client);

    rc = dixLookupDrawable(&pDraw, stuff->drawable, client, M_ANY,
                           DixGetAttrAccess);
    if (rc != Success)
        return rc;

    if (!stuff->width || !stuff->height) {
        client->errorValue = 0;
        return BadValue;
    }
    if (stuff->width > 32767 || stuff->height > 32767) {
        /* It is allowed to try and allocate a pixmap which is larger than
         * 32767 in either dimension. However, all of the framebuffer code
         * is buggy and does not reliably draw to such big pixmaps, basically
         * because the Region data structure operates with signed shorts
         * for the rectangles in it.
         *
         * Furthermore, several places in the X server computes the
         * size in bytes of the pixmap and tries to store it in an
         * integer. This integer can overflow and cause the allocated size
         * to be much smaller.
         *
         * So, such big pixmaps are rejected here with a BadAlloc
         */
        return BadAlloc;
    }
    if (stuff->depth != 1) {
        pDepth = pDraw->pScreen->allowedDepths;
        for (i = 0; i < pDraw->pScreen->numDepths; i++, pDepth++)
            if (pDepth->depth == stuff->depth)
                goto CreatePmap;
        client->errorValue = stuff->depth;
        return BadValue;
    }
 CreatePmap:
    pMap = (PixmapPtr) (*pDraw->pScreen->CreatePixmap)
        (pDraw->pScreen, stuff->width, stuff->height, stuff->depth, 0);
    if (pMap) {
        pMap->drawable.serialNumber = NEXT_SERIAL_NUMBER;
        pMap->drawable.id = stuff->pid;
        /* security creation/labeling check */
        rc = XaceHook(XACE_RESOURCE_ACCESS, client, stuff->pid, RT_PIXMAP,
                      pMap, RT_NONE, NULL, DixCreateAccess);
        if (rc != Success) {
            (*pDraw->pScreen->DestroyPixmap) (pMap);
            return rc;
        }
        if (AddResource(stuff->pid, RT_PIXMAP, (void *) pMap))
            return Success;
    }
    return BadAlloc;
}
