ProcTranslateCoords(ClientPtr client)
{
    REQUEST(xTranslateCoordsReq);

    WindowPtr pWin, pDst;
    xTranslateCoordsReply rep;
    int rc;

    REQUEST_SIZE_MATCH(xTranslateCoordsReq);
    rc = dixLookupWindow(&pWin, stuff->srcWid, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;
    rc = dixLookupWindow(&pDst, stuff->dstWid, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    rep = (xTranslateCoordsReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0
    };
    if (!SAME_SCREENS(pWin->drawable, pDst->drawable)) {
        rep.sameScreen = xFalse;
        rep.child = None;
        rep.dstX = rep.dstY = 0;
    }
    else {
        INT16 x, y;

        rep.sameScreen = xTrue;
        rep.child = None;
        /* computing absolute coordinates -- adjust to destination later */
        x = pWin->drawable.x + stuff->srcX;
        y = pWin->drawable.y + stuff->srcY;
        pWin = pDst->firstChild;
        while (pWin) {
            BoxRec box;

            if ((pWin->mapped) &&
                (x >= pWin->drawable.x - wBorderWidth(pWin)) &&
                (x < pWin->drawable.x + (int) pWin->drawable.width +
                 wBorderWidth(pWin)) &&
                (y >= pWin->drawable.y - wBorderWidth(pWin)) &&
                (y < pWin->drawable.y + (int) pWin->drawable.height +
                 wBorderWidth(pWin))
                /* When a window is shaped, a further check
                 * is made to see if the point is inside
                 * borderSize
                 */
                && (!wBoundingShape(pWin) ||
                    RegionContainsPoint(&pWin->borderSize, x, y, &box))

                && (!wInputShape(pWin) ||
                    RegionContainsPoint(wInputShape(pWin),
                                        x - pWin->drawable.x,
                                        y - pWin->drawable.y, &box))
                ) {
                rep.child = pWin->drawable.id;
                pWin = (WindowPtr) NULL;
            }
            else
                pWin = pWin->nextSib;
        }
        /* adjust to destination coordinates */
        rep.dstX = x - pDst->drawable.x;
        rep.dstY = y - pDst->drawable.y;
    }
    WriteReplyToClient(client, sizeof(xTranslateCoordsReply), &rep);
    return Success;
}
