ProcWarpPointer(ClientPtr client)
{
    WindowPtr dest = NULL;
    int x, y, rc;
    ScreenPtr newScreen;
    DeviceIntPtr dev, tmp;
    SpritePtr pSprite;

    REQUEST(xWarpPointerReq);
    REQUEST_SIZE_MATCH(xWarpPointerReq);

    dev = PickPointer(client);

    for (tmp = inputInfo.devices; tmp; tmp = tmp->next) {
        if (GetMaster(tmp, MASTER_ATTACHED) == dev) {
            rc = XaceHook(XACE_DEVICE_ACCESS, client, dev, DixWriteAccess);
            if (rc != Success)
                return rc;
        }
    }

    if (dev->lastSlave)
        dev = dev->lastSlave;
    pSprite = dev->spriteInfo->sprite;

#ifdef PANORAMIX
    if (!noPanoramiXExtension)
        return XineramaWarpPointer(client);
#endif

    if (stuff->dstWid != None) {
        rc = dixLookupWindow(&dest, stuff->dstWid, client, DixGetAttrAccess);
        if (rc != Success)
            return rc;
    }
    x = pSprite->hotPhys.x;
    y = pSprite->hotPhys.y;

    if (stuff->srcWid != None) {
        int winX, winY;
        XID winID = stuff->srcWid;
        WindowPtr source;

        rc = dixLookupWindow(&source, winID, client, DixGetAttrAccess);
        if (rc != Success)
            return rc;

        winX = source->drawable.x;
        winY = source->drawable.y;
        if (source->drawable.pScreen != pSprite->hotPhys.pScreen ||
            x < winX + stuff->srcX ||
            y < winY + stuff->srcY ||
            (stuff->srcWidth != 0 &&
             winX + stuff->srcX + (int) stuff->srcWidth < x) ||
            (stuff->srcHeight != 0 &&
             winY + stuff->srcY + (int) stuff->srcHeight < y) ||
            (source->parent && !PointInWindowIsVisible(source, x, y)))
            return Success;
    }
    if (dest) {
        x = dest->drawable.x;
        y = dest->drawable.y;
        newScreen = dest->drawable.pScreen;
    }
    else
        newScreen = pSprite->hotPhys.pScreen;

    x += stuff->dstX;
    y += stuff->dstY;

    if (x < 0)
        x = 0;
    else if (x >= newScreen->width)
        x = newScreen->width - 1;
    if (y < 0)
        y = 0;
    else if (y >= newScreen->height)
        y = newScreen->height - 1;

    if (newScreen == pSprite->hotPhys.pScreen) {
        if (x < pSprite->physLimits.x1)
            x = pSprite->physLimits.x1;
        else if (x >= pSprite->physLimits.x2)
            x = pSprite->physLimits.x2 - 1;
        if (y < pSprite->physLimits.y1)
            y = pSprite->physLimits.y1;
        else if (y >= pSprite->physLimits.y2)
            y = pSprite->physLimits.y2 - 1;
        if (pSprite->hotShape)
            ConfineToShape(dev, pSprite->hotShape, &x, &y);
        (*newScreen->SetCursorPosition) (dev, newScreen, x, y, TRUE);
    }
    else if (!PointerConfinedToScreen(dev)) {
        NewCurrentScreen(dev, newScreen, x, y);
    }
    if (*newScreen->CursorWarpedTo)
        (*newScreen->CursorWarpedTo) (dev, newScreen, client,
                                      dest, pSprite, x, y);
    return Success;
}
