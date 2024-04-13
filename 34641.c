ProcQueryBestSize(ClientPtr client)
{
    xQueryBestSizeReply reply;
    DrawablePtr pDraw;
    ScreenPtr pScreen;
    int rc;

    REQUEST(xQueryBestSizeReq);
    REQUEST_SIZE_MATCH(xQueryBestSizeReq);

    if ((stuff->class != CursorShape) &&
        (stuff->class != TileShape) && (stuff->class != StippleShape)) {
        client->errorValue = stuff->class;
        return BadValue;
    }

    rc = dixLookupDrawable(&pDraw, stuff->drawable, client, M_ANY,
                           DixGetAttrAccess);
    if (rc != Success)
        return rc;
    if (stuff->class != CursorShape && pDraw->type == UNDRAWABLE_WINDOW)
        return BadMatch;
    pScreen = pDraw->pScreen;
    rc = XaceHook(XACE_SCREEN_ACCESS, client, pScreen, DixGetAttrAccess);
    if (rc != Success)
        return rc;
    (*pScreen->QueryBestSize) (stuff->class, &stuff->width,
                               &stuff->height, pScreen);
    reply = (xQueryBestSizeReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .width = stuff->width,
        .height = stuff->height
    };
    WriteReplyToClient(client, sizeof(xQueryBestSizeReply), &reply);
    return Success;
}
