GetGeometry(ClientPtr client, xGetGeometryReply * rep)
{
    DrawablePtr pDraw;
    int rc;

    REQUEST(xResourceReq);
    REQUEST_SIZE_MATCH(xResourceReq);

    rc = dixLookupDrawable(&pDraw, stuff->id, client, M_ANY, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    rep->type = X_Reply;
    rep->length = 0;
    rep->sequenceNumber = client->sequence;
    rep->root = pDraw->pScreen->root->drawable.id;
    rep->depth = pDraw->depth;
    rep->width = pDraw->width;
    rep->height = pDraw->height;

    if (WindowDrawable(pDraw->type)) {
        WindowPtr pWin = (WindowPtr) pDraw;

        rep->x = pWin->origin.x - wBorderWidth(pWin);
        rep->y = pWin->origin.y - wBorderWidth(pWin);
        rep->borderWidth = pWin->borderWidth;
    }
    else {                      /* DRAWABLE_PIXMAP */

        rep->x = rep->y = rep->borderWidth = 0;
    }

    return Success;
}
