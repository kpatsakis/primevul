XineramaWarpPointer(ClientPtr client)
{
    WindowPtr dest = NULL;
    int x, y, rc;
    SpritePtr pSprite = PickPointer(client)->spriteInfo->sprite;

    REQUEST(xWarpPointerReq);

    if (stuff->dstWid != None) {
        rc = dixLookupWindow(&dest, stuff->dstWid, client, DixReadAccess);
        if (rc != Success)
            return rc;
    }
    x = pSprite->hotPhys.x;
    y = pSprite->hotPhys.y;

    if (stuff->srcWid != None) {
        int winX, winY;
        XID winID = stuff->srcWid;
        WindowPtr source;

        rc = dixLookupWindow(&source, winID, client, DixReadAccess);
        if (rc != Success)
            return rc;

        winX = source->drawable.x;
        winY = source->drawable.y;
        if (source == screenInfo.screens[0]->root) {
            winX -= screenInfo.screens[0]->x;
            winY -= screenInfo.screens[0]->y;
        }
        if (x < winX + stuff->srcX ||
            y < winY + stuff->srcY ||
            (stuff->srcWidth != 0 &&
             winX + stuff->srcX + (int) stuff->srcWidth < x) ||
            (stuff->srcHeight != 0 &&
             winY + stuff->srcY + (int) stuff->srcHeight < y) ||
            !XineramaPointInWindowIsVisible(source, x, y))
            return Success;
    }
    if (dest) {
        x = dest->drawable.x;
        y = dest->drawable.y;
        if (dest == screenInfo.screens[0]->root) {
            x -= screenInfo.screens[0]->x;
            y -= screenInfo.screens[0]->y;
        }
    }

    x += stuff->dstX;
    y += stuff->dstY;

    if (x < pSprite->physLimits.x1)
        x = pSprite->physLimits.x1;
    else if (x >= pSprite->physLimits.x2)
        x = pSprite->physLimits.x2 - 1;
    if (y < pSprite->physLimits.y1)
        y = pSprite->physLimits.y1;
    else if (y >= pSprite->physLimits.y2)
        y = pSprite->physLimits.y2 - 1;
    if (pSprite->hotShape)
        ConfineToShape(PickPointer(client), pSprite->hotShape, &x, &y);

    XineramaSetCursorPosition(PickPointer(client), x, y, TRUE);

    return Success;
}
